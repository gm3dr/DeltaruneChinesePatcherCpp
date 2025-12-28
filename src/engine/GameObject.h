#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include <SDL3/SDL.h>

class GameObject {
protected:
  bool active = true;

public:
  virtual ~GameObject() {}
  virtual void Update(float deltaTime) = 0;
  virtual void Draw(SDL_Renderer* renderer) = 0;
  virtual void RefreshText() {}

  void SetActive(bool state) { active = state; }
  bool IsActive() const { return active; }
};
#endif