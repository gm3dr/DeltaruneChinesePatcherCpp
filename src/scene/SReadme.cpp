#include "SReadme.h"
#include <SDL3_image/SDL_image.h>
#include <iostream>
void SReadme::Enter() {
  animBgAnim.SetAtlas(&Asset::imgBgAnim);
  animBgAnim.SetInterval(300);
  animBgAnim.SetLoop(true);
}
void SReadme::Update(int delta) {
  camera.Update(delta);
  animBgAnim.Update(delta);
}
void SReadme::Input(SDL_Event *event) {
  if (event->type == SDL_EVENT_KEY_DOWN &&
      event->key.scancode == SDL_SCANCODE_A) {
    camera.Shake();
  }
}
void SReadme::Draw(SDL_Renderer *renderer) {
  Vector2 cameraPos = camera.GetPosition();
  animBgAnim.Draw(renderer, 200 - cameraPos.x, 200 - cameraPos.y);
}
void SReadme::Exit() { std::cout << "Readme离开场景" << std::endl; }