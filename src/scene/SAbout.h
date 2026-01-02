#pragma once
#include "../engine/Scene.h"
class SAbout : public Scene {
public:
  SAbout() = default;
  ~SAbout() = default;

  void Enter() override;
  void Draw(SDL_Renderer *renderer, Camera& camera) override;
  void Update(int delta) override;
  void Input(SDL_Event *event) override;
  void Exit() override;
};