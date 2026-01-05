#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <vector>
class Atlas {
public:
  void Load(SDL_Renderer *renderer, const std::string name, int count);
  SDL_Texture *Get(int idx);
  void Add(SDL_Texture *img);
  int GetCount();
  void Clear();

private:
  std::vector<SDL_Texture *> imgList{};
};