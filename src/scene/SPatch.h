#pragma once
#include "../engine/Scene.h"
class SPatch : public Scene {
public:
  SPatch() = default;
  ~SPatch() = default;

  void Enter();
  void Draw(SDL_Renderer *renderer);
  void Update(int delta);
  void Input(SDL_Event *event);
  void Exit();
};