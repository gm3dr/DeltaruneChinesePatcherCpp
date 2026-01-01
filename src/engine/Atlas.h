#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <vector>
class Atlas {
public:
  void Load(const std::string name, int count);
  SDL_Surface* Get(int idx);
  void Add(SDL_Surface *img);
  int GetCount();
  void Clear();

private:
  std::vector<SDL_Surface*> imgList{};
};