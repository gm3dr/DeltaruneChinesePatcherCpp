#include "GameManager.h"
#include "GameConfig.h"
#include "GameObject.h"
#include "GameScene.h"
#include "GameValues.h"
#include "LangManager.h"
#include "LogManager.h"
#include <SDL3/SDL.h>
#include <string>
#include <tinyfiledialogs.h>

GameManager::GameManager()
    : settings("settings.json", Global::defaultSettings) {}

void GameManager::Init() {
  LogManager::Info("[Init] Initializing game manager...");

  if (!settings.Load()) {
    LogManager::Critical(
        "[Init] Failed to load global settings: settings.json");
    return;
  }
  LogManager::Info("[Init] Settings loaded successfully.");
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    LogManager::Critical(std::string("[Init] Failed to load graphic backend:") +
                         SDL_GetError());
    return;
  }
  int width = settings.Get<int>("baseWidth");
  int height = settings.Get<int>("baseHeight");
  window =
      SDL_CreateWindow("window", width, height, SDL_WINDOW_HIGH_PIXEL_DENSITY);
  if (!window) {
    LogManager::Critical(std::string("[Init] Failed to initialize window") +
                         SDL_GetError());
    return;
  }
  renderer = SDL_CreateRenderer(window, nullptr);
  if (!renderer) {
    LogManager::Critical(std::string("[Init] Failed to initialize renderer") +
                         SDL_GetError());
    return;
  }
  SDL_SetRenderVSync(renderer, 1);
  LogManager::Info("[Init] Window initialized: " + std::to_string(width) + "x" +
                   std::to_string(height));

  SDL_Surface *icon = SDL_LoadPNG("image/app_icon.png");
  if (icon) {
    SDL_SetWindowIcon(window, icon);
    SDL_DestroySurface(icon);
    LogManager::Info("[Init] Window icon loaded.");
  } else {
    LogManager::Warning(
        "[Init] Failed to load window icon: image/app_icon.png");
  }

  if (!LangManager::Init(settings)) {
    LogManager::Critical("[Init] Language system initialization failed.");
    return;
  }
  SDL_SetWindowTitle(window, LangManager::GetText("Meta.Title").c_str());
  LogManager::Info("[Init] Language system initialized.");
  LogManager::Info("[Init] Initialization completed.");
  running = true;
}

void GameManager::Abort() { running = false; }
GameManager::~GameManager() {
  if (currentScene) {
    delete currentScene;
    currentScene = nullptr;
  }
  if (renderer) {
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
  }
  if (window) {
    SDL_DestroyWindow(window);
    window = nullptr;
  }
  SDL_Quit();
}

GameManager *GameManager::Get() {
  static GameManager instance;
  return &instance;
}

void GameManager::ChangeLanguage(const std::string &langId) {
  LangManager::SetLanguage(langId, settings);
  SDL_SetWindowTitle(window, LangManager::GetText("Meta.Title").c_str());
  RefreshSceneText();
}

void GameManager::StepLanguage(int step) {
  LangManager::StepLanguage(step, settings);
  SDL_SetWindowTitle(window, LangManager::GetText("Meta.Title").c_str());
  RefreshSceneText();
}

std::string GameManager::GetCurrentLanguage() {
  return LangManager::GetCurrentLanguage(settings);
}

void GameManager::RefreshSceneText() {
  if (currentScene) {
    for (auto obj : currentScene->GetObjects()) {
      obj->RefreshText();
    }
  }
}

void GameManager::SetScene(GameScene *newScene) {
  if (currentScene) {
    delete currentScene;
  }
  currentScene = newScene;
  if (currentScene) {
    currentScene->Init();
  }
}

void GameManager::Run() {
  SDL_Event event;
  while (running) {
    int frameStartTime = SDL_GetTicks();
    float deltaTime = (lastTime - frameStartTime) / 1000.0f;
    lastTime = frameStartTime;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_EVENT_QUIT:
        running = false;
        break;
      }
    }
    if (!running)
      break;

    Update(deltaTime);
    Draw();
  }
}

void GameManager::Update(float dt) {
  if (currentScene) {
    currentScene->Update(dt);
  }
}

void GameManager::Draw() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // 黑色
  SDL_RenderClear(renderer);
  if (currentScene) {
    currentScene->Draw(renderer);
  }
  SDL_RenderPresent(renderer);
}