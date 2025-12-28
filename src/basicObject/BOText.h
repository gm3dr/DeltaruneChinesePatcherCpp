#ifndef O_TEXT_H
#define O_TEXT_H

#include "../engine/GameObject.h"
#include <map>
#include <string>

class BOText : public GameObject {
protected:
  std::string displayText;
  std::string langKey;
  std::map<std::string, std::string> params;

  SDL_FPoint* pos{};
  SDL_FPoint* drawPos{};
  int fontSize{};
  Color color{WHITE};
  float alpha{1.0f};
  bool centered{false};

  std::string ProcessPlaceholders(const std::string &text) const;

public:
  static constexpr int DEFAULT_FONT_SIZE = -1;

  BOText(const std::string &key, SDL_FPoint* p, Color c = WHITE, bool center = false,
        int siz = DEFAULT_FONT_SIZE, float a = 1.0f);

  ~BOText() override = default;

  void SetParam(const std::string &key, const std::string &value);
  void SetParam(const std::string &key, float value);
  void SetParam(const std::string &key, int value);

  void SetText(const std::string &newKey);
  void SetPosition(SDL_FPoint* p);
  void SetColor(Color c);
  void SetAlpha(float a);
  SDL_FPoint* GetSize() const;
  SDL_FPoint* GetPosition() const;

  void RefreshText() override;
  void Update(float deltaTime) override;
  void Draw() override;
};

#endif
