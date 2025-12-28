#include "LangManager.h"
#include "LogManager.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

TTF_Font *LangManager::mainFont = nullptr;
nlohmann::json LangManager::currentLangData;
std::vector<std::string> LangManager::langList;

LangManager::LangManager(GameConfig &cfg) {}

LangManager::~LangManager() {
  if (mainFont) {
    TTF_CloseFont(mainFont);
    mainFont = nullptr;
  }
}

bool LangManager::Init(GameConfig &settings) {
  if (!LoadFont(settings))
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

bool LangManager::LoadFont(GameConfig &settings) {
  LogManager::Info("[Lang] Loading font...");
  std::string fontPath = settings.Get<std::string>("mainFont");
  int fontSize = settings.Get<int>("mainFontSize");

  mainFont = TTF_OpenFont(fontPath.c_str(), fontSize);

  if (!mainFont) {
    LogManager::Critical("[Lang] Failed to load font: " + fontPath);
    return false;
  }

  LogManager::Info("[Lang] Font loaded: " + fontPath +
                   " (size=" + std::to_string(fontSize) + ")");
  return true;
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
TTF_Font *LangManager::GetFont() { return mainFont; }

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
