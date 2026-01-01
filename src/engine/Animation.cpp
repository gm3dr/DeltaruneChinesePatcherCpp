#include "Animation.h"
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
void Animation::Update() {}