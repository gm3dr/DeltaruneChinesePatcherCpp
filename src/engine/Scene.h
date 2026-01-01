#pragma once
#include <SDL3/SDL.h>

class Scene {
public:
  ~Scene() = default;
  Scene() = default;
  virtual void Enter() {}
  virtual void Draw(SDL_Renderer *renderer) {}
  virtual void Update(int delta) {}
  virtual void Input(SDL_Event *event) {}
  virtual void Exit() {}
};