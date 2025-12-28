#include "BOButton.h"
#include "BOImage.h"
#include "../engine/GameManager.h"

constexpr const char *BOButton::TEXTURE_PATHS[4];
constexpr SDL_Color BOButton::TEXT_COLORS[4];

BOButton::BOButton(const std::string &langKey, SDL_Rect a,
                   std::function<void()> callback, bool disabled)
    : text(langKey, {a.x, a.y}, WHITE, true), onClick(callback), area(a),
      state(disabled ? State::DISABLED : State::NORMAL) {

  for (int i = 0; i < 4; ++i) {
    images[i] = new BOImage(TEXTURE_PATHS[i], {a.x, a.y}, {a.w, a.h});
  }
  RefreshText();
}

BOButton::~BOButton() {}

void BOButton::RefreshText() {
  text.RefreshText();
  SDL_Point pos = text.GetPosition();
  pos.x = area.x + area.w / 2.0f;
  pos.y = area.y + area.h / 2.0 - text.GetSize().y / 2.0;
  text.SetPosition(pos);
}

void BOButton::SetEnabled(bool enabled) {
  if (enabled) {
    if (state == State::DISABLED) {
      state = State::NORMAL;
    }
  } else {
    state = State::DISABLED;
  }
}

void BOButton::SetPosition(SDL_Point newPos) {
  area.x = newPos.x;
  area.y = newPos.y;
  for (auto &img : images) {
    img->SetPosition(newPos);
  }
  RefreshText();
}

void BOButton::SetSize(SDL_Point newSize) {
  area.w = newSize.x;
  area.h = newSize.y;
  for (auto &img : images) {
    img->SetSize(newSize);
  }
  RefreshText();
}

void BOButton::SetCallback(std::function<void()> callback) {
  onClick = callback;
}

void BOButton::SetText(const std::string &key) {
  text.SetText(key);
  RefreshText();
}

void BOButton::Update(float dt) {
  if (state == State::DISABLED) {
    return;
  }

  float mouseX, mouseY;
  int mouseState = SDL_GetMouseState(&mouseX, &mouseY);
  float scale = SDL_GetWindowDisplayScale(GameManager::Get()->GetWindow());
  SDL_Point mousePos = {(int)(mouseX / scale), (int)(mouseY / scale)};
  bool isHovered = SDL_PointInRect(&mousePos, &area);
  bool isMouseDown = (mouseState & SDL_BUTTON_LMASK);
  bool isJustPressed = GameManager::Get()->IsMouseJustPressed();
  bool isJustReleased = GameManager::Get()->IsMouseJustReleased();

  if (isHovered) {
    if (isJustPressed) {
      state = State::PRESSED;
    } else if (isJustReleased && state == State::PRESSED) {
      if (onClick)
        onClick();
      state = State::HOVERED;
    } else if (state != State::PRESSED) {
      state = State::HOVERED;
    }
  } else {
    if (isJustReleased && state == State::PRESSED) {
      state = State::NORMAL;
    }
    if (state != State::PRESSED) {
      state = State::NORMAL;
    }
  }
}

void BOButton::Draw() {
  int stateIndex = static_cast<int>(state);
  int margin = 26;
  images[stateIndex]->Draw9Grid(margin);
  text.SetColor(TEXT_COLORS[stateIndex]);
  text.Draw();
}