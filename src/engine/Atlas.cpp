#include "Atlas.h"
#include "Game.h"
#include "SDL3_image/SDL_image.h"
#include <string>
void Atlas::Load(const std::string name, int count) {
  imgList.clear();
  imgList.resize(count);
  for (int i = 0; i < count; i++) {
    SDL_Surface *img;
    if (count == 1) {
      img = IMG_Load(("image/" + name + ".png").c_str());
    } else {
      img = IMG_Load(
          ("image/" + name + "_" + std::to_string(count) + ".png").c_str());
    }
    if (!img) {
      Game::Log("Warning", SDL_GetError());
    }
    imgList.push_back(img);
  }
}
SDL_Surface *Atlas::Get(int idx) {
  if (idx < 0 || idx >= GetCount())
    return nullptr;
  return imgList[idx];
}
void Atlas::Add(SDL_Surface *img) {
  if (img)
    imgList.push_back(img);
}
int Atlas::GetCount() { return imgList.size(); }
void Atlas::Clear() { imgList.clear(); }