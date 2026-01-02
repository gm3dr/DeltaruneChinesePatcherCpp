#pragma once
#include "engine/Animation.h"
#include "engine/Atlas.h"
#include <SDL3_ttf/SDL_ttf.h>

namespace Asset {
inline TTF_Font *fntMain;

inline Atlas imgBgAnim;
inline Atlas imgbgStatic;
inline Atlas imgBlack;
inline Atlas imgBtnDisabled;
inline Atlas imgBtnEnabled;
inline Atlas imgBtnPressed;
inline Atlas imgIcon;

inline Animation animBgAnim;
inline Animation animBgStatic;
inline Animation animBlack;
inline Animation animBtnDisabled;
inline Animation animBtnEnabled;
inline Animation animBtnPressed;
inline Animation animIcon;

inline void LoadAssets(SDL_Renderer *renderer) {
  Asset::fntMain = TTF_OpenFont("font/main.ttf", 16);

  Asset::imgBgAnim.Load(renderer, "bgAnim", 5);
  Asset::imgbgStatic.Load(renderer, "bgStatic", 1);
  Asset::imgBlack.Load(renderer, "black", 1);
  Asset::imgBtnDisabled.Load(renderer, "btnDisabled", 1);
  Asset::imgBtnEnabled.Load(renderer, "btnEnabled", 1);
  Asset::imgBtnPressed.Load(renderer, "btnPressed", 1);
  Asset::imgIcon.Load(renderer, "icon", 1);

  animBgAnim.SetAtlas(&imgBgAnim);
  animBgStatic.SetAtlas(&imgbgStatic);
  animBlack.SetAtlas(&imgBlack);
  animBtnDisabled.SetAtlas(&imgBtnDisabled);
  animBtnEnabled.SetAtlas(&imgBtnEnabled);
  animBtnPressed.SetAtlas(&imgBtnPressed);
  animIcon.SetAtlas(&imgIcon);
}
} // namespace Asset