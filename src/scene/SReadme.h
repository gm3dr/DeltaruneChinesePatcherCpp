#pragma once
#include "../engine/Animation.h"
#include "../engine/Scene.h"

class SReadme : public Scene {
public:
  SReadme() = default;
  ~SReadme() = default;

  void Enter() override;
  void Draw(SDL_Renderer *renderer) override;
  void Update(int delta) override;
  void Input(SDL_Event *event) override;
  void Exit() override;

private:
  Animation animBgAnim;
};