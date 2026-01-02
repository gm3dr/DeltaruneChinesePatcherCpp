#pragma once
#include "../Asset.h"
#include <SDL3/SDL.h>
#include "Camera.h"

class Scene {
public:
  ~Scene() = default;
  Scene() = default;
  virtual void Enter() {}
  virtual void Draw(SDL_Renderer *renderer, Camera &camera) {}
  virtual void Update(int delta) {}
  virtual void Input(SDL_Event *event) {}
  virtual void Exit() {}

public:
};