#include "BOText.h"
#include "../engine/LangManager.h"

BOText::BOText(const std::string &key, SDL_Point p, SDL_Color c, bool center,
               float s, float a)
    : pos(p), color(c), alpha(a), centered(center), scale(s) {
  SetText(key);
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
  }

  if (displayText.empty())
    displayText = " ";
  RefreshText();
}

void BOText::RefreshText() {
  auto glyphIds = DecodeUTF8(displayText);
  float totalW = 0;
  float lineHeight = (float)LangManager::GetBMLineHeight() * scale;
  uint32_t prevId = 0;

  for (uint32_t id : glyphIds) {
    const BMGlyph *g = LangManager::GetBMGlyph(id);
    if (!g)
      continue;

    totalW += (float)g->xadvance * scale;

    if (prevId != 0) {
      totalW += (float)LangManager::GetBMKerning(prevId, id) * scale;
    }
    prevId = id;
  }

  drawRect.w = (int)totalW;
  drawRect.h = (int)lineHeight;
  drawRect.x = (int)(pos.x - (centered ? (totalW / 2.0f) : 0));
  drawRect.y = pos.y;
}

std::vector<uint32_t> BOText::DecodeUTF8(const std::string &str) const {
  std::vector<uint32_t> res;
  for (size_t i = 0; i < str.length();) {
    uint32_t ch = 0;
    unsigned char c = str[i];
    if (c < 0x80) {
      ch = c;
      i += 1;
    } else if (c < 0xE0) {
      ch = ((c & 0x1F) << 6) | (str[i + 1] & 0x3F);
      i += 2;
    } else if (c < 0xF0) {
      ch =
          ((c & 0x0F) << 12) | ((str[i + 1] & 0x3F) << 6) | (str[i + 2] & 0x3F);
      i += 3;
    } else {
      ch = ((c & 0x07) << 18) | ((str[i + 1] & 0x3F) << 12) |
           ((str[i + 2] & 0x3F) << 6) | (str[i + 3] & 0x3F);
      i += 4;
    }
    res.push_back(ch);
  }
  return res;
}

void BOText::Draw() {
  SDL_Texture *tex = LangManager::GetBMTexture();
  if (!tex || displayText.empty())
    return;

  SDL_SetTextureColorMod(tex, color.r, color.g, color.b);
  SDL_SetTextureAlphaMod(tex, (Uint8)(alpha * 255.0f));

  auto glyphIds = DecodeUTF8(displayText);
  float curX = (float)drawRect.x;
  float curY = (float)drawRect.y;
  uint32_t prevId = 0;

  for (uint32_t id : glyphIds) {
    if (id == '\n') {
      curX = (float)drawRect.x;
      curY += (float)drawRect.h; // 这里的 drawRect.h 已经是缩放后的行高了
      prevId = 0;
      continue;
    }

    const BMGlyph *g = LangManager::GetBMGlyph(id);
    if (!g)
      continue;

    if (prevId != 0) {
      curX += (float)LangManager::GetBMKerning(prevId, id) * scale;
    }

    SDL_FRect src = {(float)g->x, (float)g->y, (float)g->width,
                     (float)g->height};

    // 所有的位置偏移和大小都要乘以 scale
    SDL_FRect dst = {curX + (float)g->xoffset * scale,
                     curY + (float)g->yoffset * scale, (float)g->width * scale,
                     (float)g->height * scale};

    SDL_RenderTexture(renderer, tex, &src, &dst);

    curX += (float)g->xadvance * scale;
    prevId = id;
  }
}

// 其余接口实现
void BOText::SetParam(const std::string &key, const std::string &value) {
  params[key] = value;
  std::string rawText = LangManager::GetText(langKey);
  displayText = ProcessPlaceholders(rawText);
  RefreshText();
}

void BOText::SetParam(const std::string &key, float value) {
  SetParam(key, std::to_string(value));
}
void BOText::SetParam(const std::string &key, int value) {
  SetParam(key, std::to_string(value));
}
void BOText::SetPosition(SDL_Point p) {
  pos = p;
  RefreshText();
}
SDL_Point BOText::GetPosition() const { return pos; }
void BOText::SetColor(SDL_Color c) { color = c; }
void BOText::SetAlpha(float a) { alpha = a; }
void BOText::SetScale(float s) { scale = s; }
float BOText::GetScale() const { return scale; }
void BOText::Update(float dt) {}

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