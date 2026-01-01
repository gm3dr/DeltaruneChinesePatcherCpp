#pragma once
#include "../engine/Scene.h"
class SAbout : public Scene {
public:
  SAbout() = default;
  ~SAbout() = default;

  void Enter();
  void Draw(SDL_Renderer *renderer);
  void Update();
  void Input(SDL_Event *event);
  void Exit();
};