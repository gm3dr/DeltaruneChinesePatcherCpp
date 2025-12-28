#ifndef O_TEXT_H
#define O_TEXT_H

#include "../engine/GameObject.h"
#include <map>
#include <string>

class BOText : public GameObject {
protected:
  SDL_Texture *textTexture = nullptr;
  SDL_Rect drawRect{};
  std::string displayText;
  std::string langKey;
  std::map<std::string, std::string> params;

  SDL_Point pos{};
  int fontSize{};
  SDL_Color color = WHITE;
  float alpha{1.0f};
  bool centered{false};

  std::string ProcessPlaceholders(const std::string &text) const;

public:
  static constexpr int DEFAULT_FONT_SIZE = -1;

  BOText(const std::string &key, SDL_Point p,
         SDL_Color c = WHITE, bool center = false,
         int siz = DEFAULT_FONT_SIZE, float a = 1.0f);

  ~BOText() override = default;

  void SetParam(const std::string &key, const std::string &value);
  void SetParam(const std::string &key, float value);
  void SetParam(const std::string &key, int value);

  void SetText(const std::string &newKey);
  void SetPosition(SDL_Point p);
  void SetColor(SDL_Color c);
  void SetAlpha(float a);
  SDL_Point GetSize() const;
  SDL_Point GetPosition() const;

  void RefreshText() override;
  void Update(float deltaTime) override;
  void Draw() override;
};

#endif
