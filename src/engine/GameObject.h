#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include "GameGlobal.h"
#include <SDL3/SDL.h>

class GameObject {
protected:
  bool active = true;
  SDL_Renderer *renderer = GameGlobal::renderer;
  static constexpr SDL_Color YELLOW = {255, 255, 0, 255};
  static constexpr SDL_Color WHITE = {255, 255, 255, 255};
  static constexpr SDL_Color RED = {255, 0, 0, 255};
  static constexpr SDL_Color GREEN = {0, 255, 0, 255};
  static constexpr SDL_Color GRAY = {128, 128, 128, 255};

public:
  virtual ~GameObject() {}
  virtual void Update(float deltaTime) = 0;
  virtual void Draw() = 0;
  virtual void RefreshText() {}

  void SetActive(bool state) { active = state; }
  bool IsActive() const { return active; }
};
#endif