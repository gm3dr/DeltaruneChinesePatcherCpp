#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "GameConfig.h"
#include <SDL3/SDL.h>
#include <string>

class GameScene;
class SDL_Window;

class GameManager {
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
  SDL_FPoint GetMouseWheelMove() { return mouseWheel; }
  bool IsMouseJustPressed() { return mouseJustPressed; }
  bool IsMouseJustReleased() { return mouseJustReleased; }
  void SetScene(GameScene *newScene);
  SDL_Window* GetWindow() {return window;};
  void Run();

private:
  GameScene *currentScene = nullptr;
  GameManager();
  void Update(float deltaTime);
  void Draw();
  void RefreshSceneText();

  GameConfig settings;
  SDL_Window *window;
  bool running = true;
  int lastTime;
  SDL_FPoint mouseWheel{};
  bool mouseJustReleased = false;
  bool mouseJustPressed = false;
  static constexpr int FPS = 60;
};

#endif