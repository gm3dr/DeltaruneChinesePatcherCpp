#include "SPatch.h"
#include "../engine/Game.h"
#include <SDL3_image/SDL_image.h>
#include <iostream>
void SPatch::Enter() { std::cout << "Patch初始化" << std::endl; }
void SPatch::Update(int delta) { std::cout << "Patch场景更新" << std::endl; }
void SPatch::Input(SDL_Event *event) {
  if (event->type == SDL_EVENT_KEY_DOWN &&
      event->key.scancode == SDL_SCANCODE_A) {
    Game::SwitchTo(SceneType::SReadme);
  }
}
void SPatch::Draw(SDL_Renderer *renderer) {
  SDL_RenderTexture(renderer, Asset::imgIcon.Get(0), nullptr, nullptr);
}
void SPatch::Exit() { std::cout << "Patch离开场景" << std::endl; }