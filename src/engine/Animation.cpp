#include "Animation.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include <functional>
#include <iostream>
void Animation::Reset() {
  timer = 0;
  frameIdx = 0;
}
void Animation::SetInterval(int interval) { frameInterval = interval; }
void Animation::SetLoop(bool isLoop) { loop = isLoop; }
void Animation::SetAtlas(Atlas *newAtlas) { atlas = newAtlas; }
int Animation::GetFrameIndex() { return frameIdx; }
SDL_Surface *Animation::GetFrame() { return atlas->Get(frameIdx); }
bool Animation::CheckFinished() {
  if (loop)
    return false;
  return frameInterval >= atlas->GetCount() - 1;
}
void Animation::Update(int delta) {
  timer += delta;
  if (timer >= frameInterval) {
    timer = 0;
    frameIdx++;
    if (frameIdx >= atlas->GetCount()) {
      frameIdx = loop ? 0 : atlas->GetCount() - 1;
      if (CheckFinished() && callback)
        callback();
    }
  }
}
void Animation::Draw(SDL_Renderer *renderer, int x, int y) {
  if (!renderer)
    return;
  SDL_Texture *texture =
      SDL_CreateTextureFromSurface(renderer, atlas->Get(frameInterval));
  if (!texture) {
    std::cerr << "[Warning] " << SDL_GetError() << std::endl;
    return;
  }
  float w, h;
  SDL_GetTextureSize(texture, &w, &h);
  SDL_FRect dst{x * 1.0f, y * 1.0f, w, h};
  SDL_RenderTexture(renderer, texture, nullptr, &dst);
}
void Animation::SetCallback(std::function<void()> newCallback) {
  callback = newCallback;
}