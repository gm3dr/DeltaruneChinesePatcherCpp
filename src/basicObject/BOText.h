#ifndef O_TEXT_H
#define O_TEXT_H
#include "../engine/GameObject.h"
#include <unordered_map>
#include <string>

class BOText : public GameObject {
protected:
  SDL_Rect drawRect = {0, 0, 0, 0};
  std::string displayText;
  std::string langKey;
  std::unordered_map<std::string, std::string> params;

  SDL_Point pos{};
  SDL_Color color{WHITE};
  float alpha{1.0f};
  bool centered{false};
  float scale;

  std::string ProcessPlaceholders(const std::string &text) const;
  std::vector<uint32_t> DecodeUTF8(const std::string &str) const;

public:
  static constexpr float DEFAULT_FONT_SCALE = 2.0;
  BOText(const std::string &key, SDL_Point p,
         SDL_Color c = {255, 255, 255, 255}, bool center = false, float scale = DEFAULT_FONT_SCALE,
         float a = 1.0f);
  ~BOText() override = default;

  void SetParam(const std::string &key, const std::string &value);
  void SetParam(const std::string &key, float value);
  void SetParam(const std::string &key, int value);

  void SetText(const std::string &content, bool plain = false);
  void SetPosition(SDL_Point p);
  void SetColor(SDL_Color c);
  void SetAlpha(float a);
  void SetScale();
  float GetScale() const;
  SDL_Point GetPosition() const;
  void RefreshText() override;
  void Update(float deltaTime) override;
  void Draw() override;
};
#endif