#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "GameConfig.h"
#include <string>

class GameScene;
class SDL_Window;
class SDL_Renderer;

class GameManager {
private:
  GameScene *currentScene = nullptr;

  GameManager();

public:
  ~GameManager();

  static GameManager *Get();

  void Init();
  GameConfig &Settings() { return settings; }
  GameManager(const GameManager &) = delete;
  GameManager &operator=(const GameManager &) = delete;
  void ChangeLanguage(const std::string &langId);
  void StepLanguage(int step);
  std::string GetCurrentLanguage();

  void SetScene(GameScene *newScene);
  void Run();
  void Abort();

private:
  void Update(float deltaTime);
  void Draw();
  void RefreshSceneText();

  GameConfig settings;
  SDL_Window* window;
  SDL_Renderer* renderer;
  bool running = false;
  int lastTime;
  static constexpr int FPS = 60;
};

#endif