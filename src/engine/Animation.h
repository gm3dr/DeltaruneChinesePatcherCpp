#pragma once
#include "Atlas.h"
#include <functional>
class Animation {
public:
  Animation() = default;
  ~Animation() = default;
  void Reset();
  void SetInterval(int interval);
  void SetLoop(bool isLoop);
  void SetAtlas(Atlas *newAlas);
  int GetFrameIndex();
  SDL_Texture *GetFrame();
  bool CheckFinished();
  void Update(int delta);
  void Draw(SDL_Renderer *renderer, int x, int y);
  void SetCallback(std::function<void()>);

private:
  int timer{0};
  int frameInterval{0};
  int frameIdx{0};
  bool loop{false};
  SDL_FRect drawRect;
  Atlas *atlas{nullptr};
  std::function<void()> callback;
};