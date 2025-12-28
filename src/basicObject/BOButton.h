#ifndef O_BUTTON_H
#define O_BUTTON_H

#include "../engine/GameObject.h"
#include <functional>
#include <string>

class BOButton : public GameObject {
public:
  enum class State {
    DISABLED,
    NORMAL,
    HOVERED,
    PRESSED,
  };
  BOButton(const std::string &langKey, SDL_FPoint* position, SDL_FPoint* size,
          std::function<void()> callback = nullptr, bool disabled = false);

  ~BOButton() override;
  BOButton(const BOButton &) = delete;
  BOButton &operator=(const BOButton &) = delete;
  void RefreshText() override;
  void Update(float deltaTime) override;
  void Draw() override;
  void SetEnabled(bool enabled);
  bool IsEnabled() const { return state != State::DISABLED; }
  State GetState() const { return state; }
  void SetPosition(SDL_FPoint* newPos);
  void SetSize(SDL_FPoint* newSize);
  void SetCallback(std::function<void()> callback);
  void SetText(const std::string &key);
  SDL_FPoint* GetPosition() const { return pos; }
  SDL_FPoint* GetSize() const { return size; }
  SDL_FRect* GetBounds() const { return {pos.x, pos.y, size.x, size.y}; }

private:
  static void DrawNineSlice(Texture2D tex, SDL_FRect* rect, float margin,
                            Color tint);
  SDL_FPoint* pos{};
  SDL_FPoint* size{};
  std::string langKey;
  std::string displayText;
  SDL_FPoint* cachedTextPos{};

  Texture2D textures[4]{};
  State state{State::NORMAL};
  std::function<void()> onClick;
  static constexpr int NINE_SLICE_MARGIN = 26;
  static constexpr const char *TEXTURE_PATHS[4] = {
      "image/btn_disabled.png", "image/btn_normal.png",
      "image/btn_selected.png", "image/btn_pressed.png"};
  static constexpr Color TEXT_COLORS[4] = {GRAY, WHITE, WHITE, YELLOW};
};

#endif
