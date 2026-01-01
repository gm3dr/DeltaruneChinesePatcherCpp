#pragma once
#include "../Asset.h"
#include <SDL3_ttf/SDL_ttf.h>
namespace Util {
inline void LoadAssets(SDL_Renderer* renderer) {
  Asset::fntMain = TTF_OpenFont("font/main.ttf", 16);
  Asset::imgBgAnim.Load(renderer, "bgAnim", 5);
  Asset::imgbgStatic.Load(renderer, "bgStatic", 1);
  Asset::imgBlack.Load(renderer, "black", 1);
  Asset::imgBtnDisabled.Load(renderer, "btnDisabled", 1);
  Asset::imgBtnEnabled.Load(renderer, "btnEnabled", 1);
  Asset::imgBtnPressed.Load(renderer, "btnPressed", 1);
  Asset::imgIcon.Load(renderer, "icon", 1);
}
} // namespace Util