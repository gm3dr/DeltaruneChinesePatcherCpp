#include "SReadme.h"
#include "../engine/Game.h"
#include <SDL3_image/SDL_image.h>
#include <iostream>
void SReadme::Enter() { std::cout << "Readme初始化" << std::endl; }
void SReadme::Update(int delta) { std::cout << "Readme场景更新" << std::endl; }
void SReadme::Input(SDL_Event *event) {
  if (event->type == SDL_EVENT_KEY_DOWN &&
      event->key.scancode == SDL_SCANCODE_A) {
    Game::SwitchTo(SceneType::SAbout);
  }
}
void SReadme::Draw(SDL_Renderer *renderer) {
  SDL_Texture *tex = IMG_LoadTexture(renderer, "image/bg_static.png");
  SDL_RenderTexture(renderer, tex, nullptr, nullptr);
}
void SReadme::Exit() { std::cout << "Readme离开场景" << std::endl; }