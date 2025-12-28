#ifndef O_IMAGE_H
#define O_IMAGE_H

#include "../engine/GameObject.h"
#include <string>

class BOImage : public GameObject {
protected:
  SDL_Texture *texture{};
  SDL_Point position{};
  SDL_Point size{};
  int frameCount{1};
  int frameHeight{0};
  int currentFrame{0};
  float frameTimer{0};
  float frameDuration{0};
  float alpha{1};

public:
  static constexpr int AUTO_SIZE = -1;
  BOImage(const std::string &path, SDL_Point pos,
          SDL_Point siz = {AUTO_SIZE, AUTO_SIZE},
          float alpha = 1.0);
  BOImage(const std::string &path, SDL_Point pos, int frames, float duration,
          SDL_Point siz = {AUTO_SIZE, AUTO_SIZE},
          float alpha = 1.0);

  ~BOImage() override;
  BOImage(const BOImage &) = delete;
  BOImage &operator=(const BOImage &) = delete;

  void Update(float deltaTime) override;
  void Draw() override;
  void Draw9Grid(int margin);

  SDL_Point GetSize() const;
  SDL_Point GetPosition() const;
  void SetSize(SDL_Point siz);
  void SetPosition(SDL_Point pos);
  bool IsAnimated() const { return frameCount > 1; }
};

#endif
