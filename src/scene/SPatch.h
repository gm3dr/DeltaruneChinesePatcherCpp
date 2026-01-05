#pragma once
#include "../engine/Scene.h"
class SPatch : public Scene {
public:
  SPatch() = default;
  ~SPatch() = default;

  void Enter() override;
  void Draw(SDL_Renderer *renderer, Camera& camera) override;
  void Update(int delta) override;
  void Input(SDL_Event *event) override;
  void Exit() override;
};