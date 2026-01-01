#pragma once
#include "Atlas.h"
class Animation {
public:
  Animation() = default;
  ~Animation() = default;
  void Reset();
  void SetInterval(int interval);
  void SetLoop(bool isLoop);
  void SetAtlas(Atlas *newAlas);
  int GetFrameIndex();
  SDL_Surface *GetFrame();
  bool CheckFinished();
  void Update();
  void Draw(SDL_Renderer *renderer);

private:
  int timer{0};
  int frameInterval{0};
  int frameIdx{0};
  bool loop{false};
  Atlas *atlas{nullptr};
};