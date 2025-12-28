#ifndef O_SCROLL_TEXT_H
#define O_SCROLL_TEXT_H

#include "BOText.h"

class BOScrollText : public BOText {
protected:
  int scrollOffset = 0;
  int viewHeight = 0;

public:
  BOScrollText(const std::string &key, SDL_Point p, int vh,
               SDL_Color c = WHITE, int siz = DEFAULT_FONT_SIZE);

  void Update(float deltaTime) override;
  void Draw() override;
  void SetScrollOffset(float offset);
  float GetScrollOffset() const { return scrollOffset; }
};

#endif
