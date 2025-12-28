#include "BOScrollText.h"
#include "../engine/LangManager.h"
BOScrollText::BOScrollText(const std::string &key, SDL_FRect* area, Color c,
                         int siz)
    : BOText(key, {area.x, area.y}, c, false, siz), viewRect(area) {
  CalculateHeight();
}

void BOScrollText::RefreshText() {
  BOText::RefreshText();
  CalculateHeight();
}

void BOScrollText::CalculateHeight() {
  const Font &font = LangManager::GetFont();
  SDL_FPoint* measure =
      MeasureTextEx(font, displayText.c_str(), (float)fontSize, 1.0f);
  totalContentHeight = measure.y;
}

void BOScrollText::Update(float dt) {
  SDL_FPoint* mouse = GetMousePosition();
  if (!CheckCollisionPointRec(mouse, viewRect))
    return;

  SDL_FPoint* wheelMove = GetMouseWheelMoveV();
  if (wheelMove.y != 0.0f) {
    SetScrollOffset(scrollOffset + wheelMove.y * 20.0f);
  }
}

void BOScrollText::Draw() {
  BeginScissorMode((int)viewRect.x, (int)viewRect.y, (int)viewRect.width,
                   (int)viewRect.height);

  SDL_FPoint* savedDrawPos = drawPos;
  drawPos.y += scrollOffset;

  BOText::Draw();

  drawPos = savedDrawPos;
  EndScissorMode();
}

void BOScrollText::SetScrollOffset(float offset) {
  scrollOffset = offset;
  if (scrollOffset > 0.0f) {
    scrollOffset = 0.0f;
  }
  if (totalContentHeight > viewRect.height) {
    float maxScroll = -(totalContentHeight - viewRect.height);
    if (scrollOffset < maxScroll) {
      scrollOffset = maxScroll;
    }
  } else {
    scrollOffset = 0.0f;
  }
}
