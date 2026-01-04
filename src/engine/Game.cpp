#include "Game.h"
#include "Camera.h"
#include "Scene.h"
#include <chrono>
#include <iostream>
#include <unordered_map>

SDL_Window *Game::window{nullptr};
SDL_Renderer *Game::renderer{nullptr};
SDL_Event Game::event{};
Scene *Game::currentScene{nullptr};
int Game::lastTime{0};
std::unordered_map<SceneType, Scene *> Game::sceneList{};
bool Game::running{false};
std::ofstream Game::logFile{};
Camera Game::camera{};

void Game::Init() {
  logFile = std::ofstream("log.txt");
  if (!logFile.is_open()) {
    std::cerr << "Can't create log file";
    return;
  }
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    Log("Error", SDL_GetError());
    return;
  }
  window = SDL_CreateWindow("window", BASEWIDTH, BASEHEIGHT, 0);
  if (!window) {
    Log("Error", SDL_GetError());
    return;
  }
  renderer = SDL_CreateRenderer(window, nullptr);
  if (!renderer) {
    SDL_DestroyWindow(window);
    window = nullptr;
    Log("Error", SDL_GetError());
    return;
  }
  SDL_SetRenderVSync(renderer, SDL_RENDERER_VSYNC_ADAPTIVE);
  Asset::LoadAssets(renderer);
  running = true;
}

void Game::Log(const std::string type, const std::string msg) {
  auto now = std::chrono::system_clock::now();
  std::time_t t = std::chrono::system_clock::to_time_t(now);
  logFile << "[" << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S")
          << "] ";
  logFile << type << ": " << msg << std::endl;
}

void Game::Run() {
  while (running) {
    int startTime = SDL_GetTicks();
    int delta = startTime - lastTime;
    lastTime = startTime;
    // input
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = false;
        break;
      }
      currentScene->Input(&event);
    }
    currentScene->Update(delta);
    lastTime = SDL_GetTicks();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    currentScene->Draw(renderer, camera);
    SDL_RenderPresent(renderer);
    int endTime = SDL_GetTicks();
    if (endTime - startTime <= INTERVAL) {
      SDL_Delay(INTERVAL - (endTime - startTime));
    }
  }
}

Game::~Game() {
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

void Game::SetScene(Scene *scene) { currentScene = scene; }
void Game::CreateScene(SceneType type, Scene *scene) {
  sceneList[type] = scene;
}
void Game::SwitchTo(SceneType sceneId) {
  if (currentScene)
    currentScene->Exit();
  auto result = sceneList.find(sceneId);
  if (result == sceneList.end())
    Game::Log("Warning", "Scene not found");
  currentScene = (*result).second;
  currentScene->Enter();
}