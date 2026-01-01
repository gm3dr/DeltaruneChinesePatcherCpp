#pragma once
#include "Asset.h"
#include <SDL3_ttf/SDL_ttf.h>
namespace Util {
inline void LoadAssets() {
  Asset::fntMain = TTF_OpenFont("font/main.ttf", 16);

  Asset::imgBgAnim.Load("bgAnim", 5);
  Asset::imgbgStatic.Load("bgStatic", 1);
  Asset::imgBlack.Load("black", 1);
  Asset::imgBtnDisabled.Load("btnDisabled", 1);
  Asset::imgBtnEnabled.Load("btnEnabled", 1);
  Asset::imgBtnPressed.Load("btnPressed", 1);
  Asset::imgIcon.Load("icon", 1);
}
} // namespace Util