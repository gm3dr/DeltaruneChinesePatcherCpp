#include "BOText.h"
#include "../engine/LangManager.h"
#include <SDL3_ttf/SDL_ttf.h>

BOText::BOText(const std::string &key, SDL_Point p, SDL_Color c, bool center,
               int siz, float a)
    : pos(p), color(c), alpha(a), centered(center) {

  if (siz == DEFAULT_FONT_SIZE) {
    fontSize = LangManager::GetFontSize();
  } else {
    fontSize = siz;
  }
  SetText(key);
}

std::string BOText::ProcessPlaceholders(const std::string &text) const {
  std::string result = text;
  for (const auto &[k, v] : params) {
    std::string placeholder = "{" + k + "}";
    size_t pos = 0;
    while ((pos = result.find(placeholder, pos)) != std::string::npos) {
      result.replace(pos, placeholder.length(), v);
      pos += v.length();
    }
  }
  return result;
}

void BOText::SetParam(const std::string &key, const std::string &value) {
  params[key] = value;
  RefreshText();
}

void BOText::SetParam(const std::string &key, float value) {
  SetParam(key, std::to_string(value));
}

void BOText::SetParam(const std::string &key, int value) {
  SetParam(key, std::to_string(value));
}

void BOText::SetText(const std::string &content, bool plain) {
  if (plain) {
    if (displayText != content)
      displayText = content;
  } else {
    if (langKey != content) {
      langKey = content;
      std::string rawText = LangManager::GetText(langKey);
      displayText = ProcessPlaceholders(rawText);
    }
    if (displayText == "") {
      displayText = " ";
    }
    RefreshText();
  }
}

void BOText::SetPosition(SDL_Point p) {
  pos = p;
  RefreshText();
}

SDL_Point BOText::GetPosition() const { return pos; }

void BOText::SetColor(SDL_Color c) { color = c; }

void BOText::SetAlpha(float a) { alpha = a; }

SDL_Point BOText::GetSize() const { return {drawRect.w, drawRect.h}; }

void BOText::RefreshText() {
  if (textTexture) {
    SDL_DestroyTexture(textTexture);
    textTexture = nullptr;
  }
  TTF_Font *font = LangManager::GetFont();
  int originalSize = TTF_GetFontSize(font);
  TTF_SetFontSize(font, fontSize);
  SDL_Surface *surface =
      TTF_RenderText_Solid_Wrapped(font, displayText.c_str(), 0, WHITE, 0);
  TTF_SetFontSize(font, originalSize);
  if (!surface) {
    LogManager::Error(std::string("Can't create text surface:") +
                      SDL_GetError());
    return;
  }
  textTexture = SDL_CreateTextureFromSurface(renderer, surface);
  if (!textTexture) {
    LogManager::Error(std::string("Can't create text texture:") +
                      SDL_GetError());
    SDL_DestroySurface(surface);
    return;
  }
  SDL_DestroySurface(surface);
  float w, h;
  SDL_GetTextureSize(textTexture, &w, &h);
  drawRect.w = (int)w;
  drawRect.h = (int)h;
  drawRect.x = pos.x - (centered ? drawRect.w / 2.0f : 0.0f);
  drawRect.y = pos.y;
}

void BOText::Update(float dt) {}

void BOText::Draw() {
  if (!textTexture)
    return;
  SDL_SetTextureColorMod(textTexture, color.r, color.g, color.b);
  SDL_SetTextureAlphaMod(textTexture, (Uint8)(alpha * 255));
  SDL_FRect dst = {(float)drawRect.x, (float)drawRect.y, (float)drawRect.w,
                   (float)drawRect.h};
  SDL_RenderTexture(renderer, textTexture, nullptr, &dst);
}
