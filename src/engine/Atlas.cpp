#include "Atlas.h"
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <string>
void Atlas::Load(SDL_Renderer *renderer, const std::string name, int count) {
  imgList.clear();
  imgList.resize(count);
  for (int i = 0; i < count; i++) {
    SDL_Texture *img;
    if (count == 1) {
      img = IMG_LoadTexture(renderer, ("image/" + name + ".png").c_str());
    } else {
      img = IMG_LoadTexture(
          renderer,
          ("image/" + name + "_" + std::to_string(i) + ".png").c_str());
    }
    if (!img) {
      std::cerr << "[Warning] " << SDL_GetError() << std::endl;
    }
    imgList[i] = img;
  }
}
SDL_Texture *Atlas::Get(int idx) {
  if (idx < 0 || idx >= GetCount())
    return nullptr;
  return imgList[idx];
}
void Atlas::Add(SDL_Texture *img) {
  if (img)
    imgList.push_back(img);
}
int Atlas::GetCount() { return imgList.size(); }
void Atlas::Clear() { imgList.clear(); }