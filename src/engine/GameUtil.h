#ifndef GAME_UTIL_H
#define GAME_UTIL_H

#include "../extra/WindowsIncludeFix.h"
#include <SDL3/SDL_pixels.h>
#include <filesystem>
#include <string>

namespace GameUtil {
std::string CalcFileSha256(const std::string &path);
std::string ConvertPath(const std::filesystem::path &p);
SDL_Color Fade(SDL_Color color, float alpha);
#ifdef _WIN32
std::wstring Utf8ToWide(const std::string &s);
std::string WideToUtf8(const std::wstring &ws);
std::string GetRegValue(HKEY hKey, const std::string &subKey,
                        const std::string &value);
#endif
} // namespace GameUtil
#endif