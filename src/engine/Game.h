#pragma once
#include "SDL3/SDL_render.h"
#include <SDL3/SDL.h>
#include <fstream>
#include <string>
#include <unordered_map>
class Scene;
enum class SceneType;
class Game {
private:
  Game(const Game &) = delete;
  Game &operator=(const Game &) = delete;

  ~Game();
  static SDL_Window *window;
  static SDL_Renderer *renderer;
  static SDL_Event event;
  static Scene *currentScene;
  static std::unordered_map<SceneType, Scene *> sceneList;
  static bool running;
  static std::ofstream logFile;
  constexpr static int FPS{5};
  constexpr static int BASEWIDTH{960};
  constexpr static int BASEHEIGHT{720};

  static void Draw(SDL_Renderer *renderer);
  static void Update();
  static void Input(SDL_Event *event);

public:
  static void Init();
  static void Run();
  static void Log(const std::string type, const std::string msg);
  static void SetScene(Scene *scene);
  static void CreateScene(SceneType type, Scene *scene);
  static void SwitchTo(SceneType type);
  static SDL_Renderer* GetRenderer();
};

enum class SceneType { SReadme, SPatch, SAbout };