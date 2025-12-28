#include "BOScrollText.h"
#include "../engine/GameManager.h"
BOScrollText::BOScrollText(const std::string &key, SDL_Point p, int vh,
                           SDL_Color c, int siz)
    : BOText(key, p, c, false, siz), viewHeight(vh) {}

void BOScrollText::Update(float dt) {
  float mouseX, mouseY;
  SDL_GetMouseState(&mouseX, &mouseY);

  SDL_Point mousePos = {(int)mouseX, (int)mouseY};
  SDL_Rect viewRect = {drawRect.x, drawRect.y, drawRect.w, viewHeight};

  if (!SDL_PointInRect(&mousePos, &viewRect))
    return;

  float moveY = GameManager::Get()->GetMouseWheelMove().y;
  if (moveY != 0.0f) {
    SetScrollOffset(scrollOffset + moveY * 20.0f);
  }
}

void BOScrollText::Draw() {
  if (!textTexture)
    return;
  SDL_FRect src = {0, (float)-scrollOffset, (float)drawRect.w,
                   (float)viewHeight};
  SDL_FRect dst = {(float)drawRect.x, (float)drawRect.y, (float)drawRect.w,
                   (float)viewHeight};

  SDL_SetTextureColorMod(textTexture, color.r, color.g, color.b);
  SDL_SetTextureAlphaMod(textTexture, (Uint8)(alpha * 255));
  SDL_RenderTexture(renderer, textTexture, &src, &dst);
}

void BOScrollText::SetScrollOffset(float offset) {
  scrollOffset = offset;
  if (scrollOffset > 0.0f) {
    scrollOffset = 0.0f;
  }
  if (drawRect.h > viewHeight) {
    float maxScroll = -(drawRect.h - viewHeight);
    if (scrollOffset < maxScroll) {
      scrollOffset = maxScroll;
    }
  } else {
    scrollOffset = 0.0f;
  }
}
