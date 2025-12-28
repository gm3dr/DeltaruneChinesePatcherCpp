#ifndef GAME_VALUES_H
#define GAME_VALUES_H
#include <nlohmann/json.hpp>
#include <SDL3/SDL.h>
class GameGlobal {
public:
  static SDL_Renderer *renderer;
  static const nlohmann::json defaultSettings;
};
#endif
