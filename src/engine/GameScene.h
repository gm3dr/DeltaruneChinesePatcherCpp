#ifndef GAME_SCENE_H
#define GAME_SCENE_H
#include <SDL3/SDL.h>
#include <vector>
#include "GameManager.h"

class GameObject;

class GameScene {
protected:
  std::vector<GameObject *> objects;
  std::vector<GameObject *> objectsToAdd;
  std::vector<GameObject *> objectsToRemove;
  void FlushObjects();
  static constexpr SDL_Color YELLOW = {255, 255, 0, 255};
  static constexpr SDL_Color WHITE = {255, 255, 255, 255};
  static constexpr SDL_Color RED = {255, 0, 0, 255};
  static constexpr SDL_Color GREEN = {0, 255, 0, 255};
  static constexpr SDL_Color GRAY = {128, 128, 128, 255};

public:
  virtual ~GameScene();

  virtual void Init() = 0;
  virtual void Update(float deltaTime);
  virtual void Draw();

  void AddObject(GameObject *obj);
  void AddObjects(const std::vector<GameObject *> &src);
  void RemoveObject(GameObject *obj);
  void ClearObjects();

  const std::vector<GameObject *> &GetObjects() const;
};

#endif
