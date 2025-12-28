#ifndef O_IMAGE_H
#define O_IMAGE_H

#include "../engine/GameObject.h"
#include <string>

class BOImage : public GameObject {
private:
  Texture2D texture{};
  SDL_FPoint* position{};
  SDL_FPoint* size{};
  Color tint{WHITE};
  int frameCount{1};
  float frameHeight{0};
  int currentFrame{0};
  float frameTimer{0};
  float frameDuration{0};

public:
  static constexpr float AUTO = -1.0f;
  BOImage(std::string path, SDL_FPoint* pos, SDL_FPoint* siz = {AUTO, AUTO},
         Color color = WHITE);
  BOImage(std::string path, SDL_FPoint* pos, int frames, float duration,
         SDL_FPoint* siz = {AUTO, AUTO}, Color color = WHITE);

  ~BOImage() override;
  BOImage(const BOImage &) = delete;
  BOImage &operator=(const BOImage &) = delete;

  void Update(float deltaTime) override;
  void Draw() override;

  SDL_FPoint* GetSize() const;
  void SetPosition(SDL_FPoint* pos);
  bool IsAnimated() const { return frameCount > 1; }
};

#endif
