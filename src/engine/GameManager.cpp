#include "GameManager.h"
#include "GameConfig.h"
#include "GameGlobal.h"
#include "GameObject.h"
#include "GameScene.h"
#include "LangManager.h"
#include "LogManager.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

SDL_Renderer *GameGlobal::renderer = nullptr;
const nlohmann::json GameGlobal::defaultSettings = {
    {"patcherVersion", "0.9.1"},
    {"displayLanguage", "zh_CN"},
    {"baseWidth", 960},
    {"baseHeight", 720},
    {"qqGroupCode", "1033065757"},
    {"mainFont", "external/main.ttf"},
    {"mainFontSize", 32},
    {"deltaruneAppId", "1671210"},
    {"deltaruneDepotId", "1671212"},
    {"patchRemoteUrl", "https://git.ws3917.space:29271/ws3917/"
                       "deltarune_cnpatch/raw/branch/main"},
    {"patchFileXDelta",
     {
         {"data.win", "main.xdelta"},
         {"chapter1_windows/data.win", "chapter1.xdelta"},
         {"chapter2_windows/data.win", "chapter2.xdelta"},
         {"chapter3_windows/data.win", "chapter3.xdelta"},
         {"chapter4_windows/data.win", "chapter4.xdelta"},
     }},
    {
        "backupFiles",
        {"data.win", "chapter1_windows/data.win", "chapter2_windows/data.win",
         "chapter3_windows/data.win", "chapter4_windows/data.win",
         "chapter3_windows/vid/tennaIntroF1_compressed_28.mp4"},
    },
    {"patchFileStatic",
     {
         "chapter1_windows/lang/lang_en.json",
         "chapter1_windows/lang/lang_en_names.json",
         "chapter1_windows/lang/lang_en_names_recruitable.json",
         "chapter2_windows/lang/lang_en.json",
         "chapter2_windows/lang/lang_en_names.json",
         "chapter2_windows/lang/lang_en_names_recruitable.json",
         "chapter3_windows/lang/lang_en.json",
         "chapter3_windows/lang/lang_en_names.json",
         "chapter3_windows/lang/lang_en_names_recruitable.json",
         "chapter3_windows/vid/tennaIntroF1_compressed_28.mp4",
         "chapter3_windows/vid/tennaIntroF1_zhname_compressed_28.mp4",
         "chapter4_windows/lang/lang_en.json",
         "chapter4_windows/lang/lang_en_names.json",
         "chapter4_windows/lang/lang_en_names_recruitable.json",
     }},
    {"toolSha256",
     {
#ifdef _WIN32
         {"external/win/7z.exe",
          "d2c0045523cf053a6b43f9315e9672fc2535f06aeadd4ffa53c729cd8b2b6dfe"},
         {"external/win/hpatchz.exe",
          "6855c01cf4a1662ba421e6f95370cf9afa2b3ab6c148473c63efe60d634dfb9a"},
#else
         {"external/linux/7z",
          "9a556170350dafb60a97348b86a94b087d97fd36007760691576cac0d88b132b"},
         {"external/linux/xdelta3",
          "709f63ebb9655dc3b5c84f17e11217494eb34cf0009a026386e4c8617ea903"},
#endif
     }}};

GameManager::GameManager()
    : settings("settings.json", GameGlobal::defaultSettings) {}

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
  if (!TTF_Init()) {
    LogManager::Critical(std::string("[Init] Failed to load font backend:") +
                         SDL_GetError());
    return;
  }
  int width = settings.Get<int>("baseWidth");
  int height = settings.Get<int>("baseHeight");
  float scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
  window = SDL_CreateWindow("window", scale * width, scale * height,
                            SDL_WINDOW_HIGH_PIXEL_DENSITY);
  if (!window) {
    LogManager::Critical(std::string("[Init] Failed to initialize window") +
                         SDL_GetError());
    return;
  }
  GameGlobal::renderer = SDL_CreateRenderer(window, nullptr);
  if (!GameGlobal::renderer) {
    LogManager::Critical(std::string("[Init] Failed to initialize renderer") +
                         SDL_GetError());
    return;
  }
  SDL_SetRenderVSync(GameGlobal::renderer, 1);
  SDL_SetDefaultTextureScaleMode(GameGlobal::renderer, SDL_SCALEMODE_PIXELART);
  SDL_SetRenderLogicalPresentation(GameGlobal::renderer, width, height,
                                   SDL_LOGICAL_PRESENTATION_LETTERBOX);
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

GameManager::~GameManager() {
  if (currentScene) {
    delete currentScene;
    currentScene = nullptr;
  }
  if (GameGlobal::renderer) {
    SDL_DestroyRenderer(GameGlobal::renderer);
    GameGlobal::renderer = nullptr;
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
    if (LogManager::HasCritical()) {
      running = false;
      SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error",
                               LogManager::GetCriticalMsg().c_str(), window);
    }
    int frameStartTime = SDL_GetTicks();
    float deltaTime = (lastTime - frameStartTime) / 1000.0f;
    lastTime = frameStartTime;
    mouseJustPressed = false;
    mouseJustReleased = false;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_EVENT_QUIT:
        running = false;
        break;
      case SDL_EVENT_MOUSE_WHEEL:
        mouseWheel.x = event.wheel.x;
        mouseWheel.y = event.wheel.y;
        break;
      case SDL_EVENT_MOUSE_BUTTON_DOWN:
        if (event.button.button == SDL_BUTTON_LEFT) {
          mouseJustPressed = true;
        }
        break;
      case SDL_EVENT_MOUSE_BUTTON_UP:
        if (event.button.button == SDL_BUTTON_LEFT) {
          mouseJustReleased = true;
        }
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
  SDL_SetRenderDrawColor(GameGlobal::renderer, 0, 0, 0, 255); // 黑色
  SDL_RenderClear(GameGlobal::renderer);
  if (currentScene) {
    currentScene->Draw();
  }
  SDL_RenderPresent(GameGlobal::renderer);
}