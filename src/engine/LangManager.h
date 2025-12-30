#ifndef LANG_MANAGER_H
#define LANG_MANAGER_H

#include "GameConfig.h"
#include <SDL3/SDL.h>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <vector>

// --- BMF 结构定义 (按文档对齐) ---
struct BMGlyph {
  uint32_t id;
  uint16_t x, y;
  uint16_t width, height;
  int16_t xoffset, yoffset;
  int16_t xadvance;
  uint8_t page;
  uint8_t chnl;
};

struct BMCommon {
  uint16_t lineHeight;
  uint16_t base;
  uint16_t scaleW, scaleH;
  uint16_t pages;
  uint8_t packed;
};

struct BMKerning {
  int16_t amount;
};

// 组合键用于 Kerning Map
struct KerningKey {
  uint32_t first;
  uint32_t second;
  bool operator==(const KerningKey &other) const {
    return first == other.first && second == other.second;
  }
};
struct KerningKeyHash {
  std::size_t operator()(const KerningKey &k) const {
    return (size_t)k.first ^ ((size_t)k.second << 16);
  }
};

class LangManager {
public:
  LangManager(GameConfig &cfg);
  ~LangManager();

  static bool Init(SDL_Renderer *renderer, GameConfig &settings);
  // --- 新增：位图字体接口 ---
  static const BMGlyph *GetBMGlyph(uint32_t id);            // 获取单个字符数据
  static int GetBMKerning(uint32_t first, uint32_t second); // 获取字间距
  static SDL_Texture *GetBMTexture();                       // 获取图集纹理
  static int GetBMLineHeight();                             // 获取行高

  // 语言切换相关
  static void SetLanguage(const std::string &langId, GameConfig &settings);
  static std::string GetCurrentLanguage(GameConfig &settings);
  static void StepLanguage(int step, GameConfig &settings);
  static std::string GetText(const std::string &key);
  static const std::vector<std::string> &GetAvailableLanguages();

private:
  static bool LoadFont(SDL_Renderer *renderer, GameConfig &settings);
  static bool LoadBitmapFont(SDL_Renderer *renderer,
                             const std::string &fntPath);
  static bool LoadLanguageList();
  static bool LoadLanguageData(const std::string &langId);

  static nlohmann::json currentLangData;
  static std::vector<std::string> langList;

  // --- 新增：位图字体数据 ---
  static BMCommon bmCommon;
  static std::unordered_map<uint32_t, BMGlyph> bmGlyphs;
  static std::unordered_map<KerningKey, int16_t, KerningKeyHash> bmKernings;
  static SDL_Texture *bmTexture;
};
#endif