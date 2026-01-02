#pragma once
#include "../engine/Camera.h"
#include "../engine/Scene.h"

class SReadme : public Scene {
public:
  SReadme() = default;
  ~SReadme() = default;

  void Enter() override;
  void Draw(SDL_Renderer *renderer, Camera &camera) override;
  void Update(int delta) override;
  void Input(SDL_Event *event) override;
  void Exit() override;

private:
  Vector2 textReadmeTitlePos{};
  const int BTN_OFFSET_Y{80};
  const Vector2 BTN_SIZE{200, 80};
};