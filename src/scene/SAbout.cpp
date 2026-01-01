#include "SAbout.h"
#include "../engine/Game.h"
#include <SDL3_image/SDL_image.h>
#include <iostream>
void SAbout::Enter() { std::cout << "SAbout初始化" << std::endl; }
void SAbout::Update(int delta) { std::cout << "SAbout场景更新" << std::endl; }
void SAbout::Input(SDL_Event *event) {
  if (event->type == SDL_EVENT_KEY_DOWN && event->key.scancode == SDL_SCANCODE_C)
  {
    Game::SwitchTo(SceneType::SPatch);
  }
}
void SAbout::Draw(SDL_Renderer *renderer) {
  SDL_Texture *tex = IMG_LoadTexture(renderer, "image/black.png");
  SDL_RenderTexture(renderer, tex, nullptr, nullptr);
}
void SAbout::Exit() { std::cout << "SAbout离开场景" << std::endl; }