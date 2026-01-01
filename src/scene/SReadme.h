#pragma once
#include "../engine/Scene.h"
class SReadme : public Scene {
public:
  SReadme() = default;
  ~SReadme() = default;

  void Enter();
  void Draw(SDL_Renderer *renderer);
  void Update(int delta);
  void Input(SDL_Event *event);
  void Exit();
};