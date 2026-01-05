#include "Animation.h"
void Animation::Reset() {
  timer = 0;
  frameIdx = 0;
}
void Animation::SetInterval(int interval) { frameInterval = interval; }
void Animation::SetLoop(bool isLoop) { loop = isLoop; }
void Animation::SetAtlas(Atlas *newAtlas) { atlas = newAtlas; }
int Animation::GetFrameIndex() { return frameIdx; }
SDL_Texture *Animation::GetFrame() { return atlas->Get(frameIdx); }
bool Animation::CheckFinished() {
  if (loop)
    return false;
  return frameIdx >= atlas->GetCount() - 1;
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
  if (!renderer || !atlas)
    return;
  SDL_Texture *tex = atlas->Get(frameIdx);
  SDL_GetTextureSize(tex, &drawRect.w, &drawRect.h);
  drawRect.x = (float)x;
  drawRect.y = (float)y;
  SDL_RenderTexture(renderer, tex, nullptr, &drawRect);
}
void Animation::SetCallback(std::function<void()> newCallback) {
  callback = newCallback;
}