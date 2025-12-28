#ifndef O_BUTTON_H
#define O_BUTTON_H

#include "../engine/GameObject.h"
#include "BOImage.h"
#include "BOText.h"
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
  BOButton(const std::string &langKey, SDL_Rect area,
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
  void SetPosition(SDL_Point newPos);
  void SetSize(SDL_Point newSize);
  void SetCallback(std::function<void()> callback);
  void SetText(const std::string &key);
  SDL_Point GetPosition() const { return {area.x, area.y}; }
  SDL_Point GetSize() const { return {area.x, area.y}; }
  SDL_Rect GetBounds() const { return area; }

protected:
  State state{State::NORMAL};
  SDL_Rect area;
  BOText text;
  BOImage *images[4]{};
  std::function<void()> onClick;
  static constexpr int NINE_SLICE_MARGIN = 26;
  static constexpr const char *TEXTURE_PATHS[4] = {
      "image/btn_disabled.png", "image/btn_normal.png",
      "image/btn_selected.png", "image/btn_pressed.png"};
  // GRAY, WHITE, WHITE< YELLOW
  static constexpr SDL_Color TEXT_COLORS[4] = { GRAY, WHITE, WHITE, YELLOW};
};

#endif
