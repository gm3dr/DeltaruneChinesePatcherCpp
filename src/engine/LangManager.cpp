#include "LangManager.h"
#include "LogManager.h"
#include <SDL3_image/SDL_image.h> // 需要 SDL_image 来加载 PNG
#include <cstring>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

// 初始化静态成员
nlohmann::json LangManager::currentLangData;
std::vector<std::string> LangManager::langList;
BMCommon LangManager::bmCommon = {};
std::unordered_map<uint32_t, BMGlyph> LangManager::bmGlyphs;
std::unordered_map<KerningKey, int16_t, KerningKeyHash> LangManager::bmKernings;
SDL_Texture *LangManager::bmTexture = nullptr;

LangManager::LangManager(GameConfig &cfg) {}

LangManager::~LangManager() {
  SDL_DestroyTexture(bmTexture);
  bmTexture = nullptr;
}
bool LangManager::Init(SDL_Renderer *renderer, GameConfig &settings) {
  if (!LoadFont(renderer, settings))
    return false;
  if (!LoadLanguageList())
    return false;

  std::string savedLang = settings.Get<std::string>("displayLanguage");
  auto it = std::find(langList.begin(), langList.end(), savedLang);
  if (it == langList.end() && !langList.empty()) {
    savedLang = langList[0];
    settings.Set("displayLanguage", savedLang);
  }

  if (!LoadLanguageData(savedLang))
    return false;
  return true;
}

bool LangManager::LoadFont(SDL_Renderer *renderer, GameConfig &settings) {
  std::string fontPath("font/main.fnt");
  SDL_DestroyTexture(bmTexture);
  bmTexture = nullptr;
  bmGlyphs.clear();
  bmKernings.clear();

  fs::path path(fontPath);
  std::string ext = path.extension().string();
  LogManager::Info("[Lang] Detected Bitmap Font: " + fontPath);
  return LoadBitmapFont(renderer, fontPath);
}

bool LangManager::LoadLanguageData(const std::string &langId) {
  fs::path langPath = fs::path("lang") / (langId + ".json");
  LogManager::Info("[Lang] Loading language file: " + langPath.string());

  if (!fs::exists(langPath)) {
    LogManager::Critical("[Lang] Language file not found: " +
                         langPath.string());
    currentLangData = nlohmann::json::object();
    return false;
  }

  try {
    std::ifstream i(langPath);
    if (!i) {
      LogManager::Critical("[Lang] Failed to open: " + langPath.string());
      return false;
    }
    i >> currentLangData;
  } catch (const std::exception &e) {
    LogManager::Critical("[Lang] Failed to parse json: " +
                         std::string(e.what()));
    return false;
  }

  LogManager::Info("[Lang] Language loaded: " + langId);
  return true;
}

bool LangManager::LoadLanguageList() {
  langList.clear();
  fs::path langDir = "lang";

  LogManager::Info("[Lang] Scanning language directory...");

  if (!fs::exists(langDir)) {
    LogManager::Critical("[Lang] Language directory not found: " +
                         langDir.string());
    return false;
  }

  try {
    for (const auto &entry : fs::directory_iterator(langDir)) {
      if (entry.path().extension() == ".json") {
        langList.push_back(entry.path().stem().string());
      }
    }
  } catch (const std::exception &e) {
    LogManager::Critical("[Lang] Failed to scan lang directory: " +
                         std::string(e.what()));
    return false;
  }

  std::sort(langList.begin(), langList.end());

  if (langList.empty()) {
    LogManager::Critical("[Lang] No language json files found in: " +
                         langDir.string());
    return false;
  }

  LogManager::Info("[Lang] Found " + std::to_string(langList.size()) +
                   " languages.");
  return true;
}

// ---------------------------------------------------------
//  二进制 FNT 解析核心逻辑
// ---------------------------------------------------------
bool LangManager::LoadBitmapFont(SDL_Renderer *renderer,
                                 const std::string &fntPath) {
  std::ifstream file(fntPath, std::ios::binary);
  if (!file) {
    LogManager::Critical("[Lang] Failed to open FNT file: " + fntPath);
    return false;
  }

  // 1. 检查头部 "BMF" + Version 3
  char magic[3];
  file.read(magic, 3);
  char version;
  file.read(&version, 1);

  if (strncmp(magic, "BMF", 3) != 0 || version != 3) {
    LogManager::Critical("[Lang] Invalid BMF format or version (must be v3).");
    return false;
  }

  fs::path fontDir = fs::path(fntPath).parent_path();

  // 循环读取 Block
  while (file.peek() != EOF) {
    uint8_t blockType;
    uint32_t blockSize;

    file.read((char *)&blockType, 1);
    file.read((char *)&blockSize, 4);

    // 记录块开始位置，确保读取固定长度后跳到下一块
    std::streampos blockStart = file.tellg();

    switch (blockType) {
    case 1: // Info Block
      break;
    case 2: // Common Block
    {
      file.read((char *)&bmCommon.lineHeight, 2);
      file.read((char *)&bmCommon.base, 2);
      file.read((char *)&bmCommon.scaleW, 2);
      file.read((char *)&bmCommon.scaleH, 2);
      file.read((char *)&bmCommon.pages, 2);
      file.read((char *)&bmCommon.packed, 1);
      // 忽略后续 color channels
      break;
    }
    case 3: // Pages Block (Texture filenames)
    {
      std::vector<char> buffer(blockSize);
      file.read(buffer.data(), blockSize);

      std::string pageName;
      int start = 0;
      for (size_t i = 0; i < blockSize; ++i) {
        if (buffer[i] == '\0') {
          pageName = std::string(&buffer[start]);

          // 加载纹理
          fs::path texPath = fontDir / pageName;
          SDL_Texture *tex =
              IMG_LoadTexture(renderer, texPath.string().c_str());
          if (!tex) {
            LogManager::Error("[Lang] Failed to load font texture: " +
                              texPath.string());
          } else {
            bmTexture = tex;
          }

          start = i + 1;
        }
      }
      break;
    }
    case 4: // Chars Block
    {
      int numChars = blockSize / 20;
      for (int i = 0; i < numChars; ++i) {
        BMGlyph g;
        file.read((char *)&g.id, 4);
        file.read((char *)&g.x, 2);
        file.read((char *)&g.y, 2);
        file.read((char *)&g.width, 2);
        file.read((char *)&g.height, 2);
        file.read((char *)&g.xoffset, 2);
        file.read((char *)&g.yoffset, 2);
        file.read((char *)&g.xadvance, 2);
        file.read((char *)&g.page, 1);
        file.read((char *)&g.chnl, 1);

        bmGlyphs[g.id] = g;
      }
      break;
    }
    case 5: // Kerning Block
    {
      int numPairs = blockSize / 10;
      for (int i = 0; i < numPairs; ++i) {
        uint32_t first, second;
        int16_t amount;
        file.read((char *)&first, 4);
        file.read((char *)&second, 4);
        file.read((char *)&amount, 2);

        bmKernings[{first, second}] = amount;
      }
      break;
    }
    default:
      // 未知块，跳过
      break;
    }

    // 移动到下一个块的开头
    file.seekg(blockStart + (std::streamoff)blockSize);
  }
  LogManager::Info("[Lang] Loaded Bitmap Font with " +
                   std::to_string(bmGlyphs.size()) + " chars.");
  return true;
}

// --- Getter 实现 ---

const BMGlyph *LangManager::GetBMGlyph(uint32_t id) {
  auto it = bmGlyphs.find(id);
  if (it != bmGlyphs.end())
    return &it->second;
  return nullptr;
}

int LangManager::GetBMKerning(uint32_t first, uint32_t second) {
  auto it = bmKernings.find({first, second});
  if (it != bmKernings.end())
    return it->second;
  return 0;
}

SDL_Texture *LangManager::GetBMTexture() { return bmTexture; }

int LangManager::GetBMLineHeight() { return bmCommon.lineHeight; }

std::string LangManager::GetText(const std::string &key) {
  nlohmann::json *current = &currentLangData;
  std::string token;
  std::stringstream ss(key);

  while (std::getline(ss, token, '.')) {
    if (current->contains(token)) {
      current = &((*current)[token]);
    } else {
      return key;
    }
  }

  if (current->is_string()) {
    return current->get<std::string>();
  }

  return key;
}

void LangManager::SetLanguage(const std::string &langId, GameConfig &settings) {
  auto it = std::find(langList.begin(), langList.end(), langId);
  if (it != langList.end()) {
    settings.Set("displayLanguage", langId);
    LoadLanguageData(langId);
  }
}

std::string LangManager::GetCurrentLanguage(GameConfig &settings) {
  return settings.Get<std::string>("displayLanguage");
}

void LangManager::StepLanguage(int step, GameConfig &settings) {
  if (langList.empty())
    return;

  std::string currentLang = GetCurrentLanguage(settings);
  auto it = std::find(langList.begin(), langList.end(), currentLang);

  if (it == langList.end()) {
    it = langList.begin();
  } else if (step > 0) {
    ++it;
    if (it == langList.end())
      it = langList.begin();
  } else if (step < 0) {
    if (it == langList.begin())
      it = langList.end();
    --it;
  }

  SetLanguage(*it, settings);
}

const std::vector<std::string> &LangManager::GetAvailableLanguages() {
  return langList;
}
