#pragma once
#include "Atlas.h"
namespace Util {
inline void LoadAssets() {
    Atlas imgBgAnim;
    Atlas imgbgStatic;
    Atlas imgBlack;
    Atlas imgBtnDisabled;
    Atlas imgBtnEnabled;
    Atlas imgBtnPressed;
    Atlas imgIcon;

    imgBgAnim.Load("bgAnim", 5);
    imgbgStatic.Load("bgStatic", 1);
    imgBlack.Load("black", 1);
    imgBtnDisabled.Load("btnDisabled", 1);
    imgBtnEnabled.Load("btnEnabled", 1);
    imgBtnPressed.Load("btnPressed", 1);
    imgIcon.Load("icon", 1);
}
} // namespace Util