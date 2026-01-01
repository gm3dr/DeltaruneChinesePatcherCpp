#include "SReadme.h"
#include "../engine/Game.h"
#include <SDL3_image/SDL_image.h>
#include <iostream>
void SReadme::Enter() {
  animBgAnim.SetAtlas(&Asset::imgBgAnim);
  animBgAnim.SetInterval(300);
  animBgAnim.SetLoop(true);
}
void SReadme::Update(int delta) { animBgAnim.Update(delta); }
void SReadme::Input(SDL_Event *event) {
  if (event->type == SDL_EVENT_KEY_DOWN &&
      event->key.scancode == SDL_SCANCODE_A) {
    Game::SwitchTo(SceneType::SAbout);
  }
}
void SReadme::Draw(SDL_Renderer *renderer) {
  animBgAnim.Draw(renderer, 200, 200);
}
void SReadme::Exit() { std::cout << "Readme离开场景" << std::endl; }