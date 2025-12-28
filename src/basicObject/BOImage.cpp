#include "BOImage.h"
#include "../engine/LogManager.h"
#include <SDL3_image/SDL_image.h>

BOImage::BOImage(const std::string &path, SDL_Point pos, SDL_Point siz, float a)
    : position(pos), alpha(a), frameCount(1) {

  texture = IMG_LoadTexture(renderer, path.c_str());

  if (!texture) {
    LogManager::Warning("[Image] Failed to load texture: " + path);
    size = {0, 0};
    return;
  }
  SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
  float tw, th;
  SDL_GetTextureSize(texture, &tw, &th);
  SDL_SetTextureAlphaMod(texture, (Uint8)(alpha * 255));
  size = {siz.x < 0 ? (int)tw : siz.x, siz.y < 0 ? (int)th : siz.y};
}

BOImage::BOImage(const std::string &path, SDL_Point pos, int frames,
                 float duration, SDL_Point siz, float a)
    : position(pos), alpha(a), frameCount(frames), frameDuration(duration),
      currentFrame(0), frameTimer(0.0f) {

  texture = IMG_LoadTexture(renderer, path.c_str());

  if (!texture) {
    LogManager::Warning("[Image] Failed to load texture: " + path);
    frameHeight = 0;
    size = {0, 0};
    return;
  }

  float tw, th;
  SDL_GetTextureSize(texture, &tw, &th);

  frameHeight = (int)th / frameCount;
  size = {siz.x < 0 ? (int)tw : siz.x, siz.y < 0 ? (int)th : siz.y};
}

BOImage::~BOImage() {
  if (texture) {
    SDL_DestroyTexture(texture);
  }
}

void BOImage::Update(float deltaTime) {

  if (frameCount <= 1)
    return;

  frameTimer += deltaTime;
  if (frameTimer >= frameDuration) {
    frameTimer -= frameDuration;
    currentFrame = (currentFrame + 1) % frameCount;
  }
}

void BOImage::Draw() {
  if (!texture)
    return;

  int srcY = frameCount > 1 ? currentFrame * frameHeight : 0;
  float tw, th;
  SDL_GetTextureSize(texture, &tw, &th);

  int srcHeight = frameCount > 1 ? frameHeight : (int)th;

  SDL_FRect source = {0.0f, (float)srcY, tw, (float)srcHeight};
  SDL_FRect dest = {(float)position.x, (float)position.y, (float)size.x,
                    (float)size.y};

  SDL_RenderTexture(renderer, texture, &source, &dest);
}

void BOImage::Draw9Grid(int margin) {
  if (!texture)
    return;

  int srcY = frameCount > 1 ? currentFrame * frameHeight : 0;
  int srcHeight = frameCount > 1 ? frameHeight : texture->h;

  SDL_FRect source = {0, (float)srcY, (float)texture->w, (float)srcHeight};
  SDL_FRect dest = {(float)position.x, (float)position.y, (float)size.x,
                    (float)size.y};
  SDL_RenderTexture9Grid(renderer, texture, &source, margin, margin, margin,
                         margin, 1, &dest);
}

SDL_Point BOImage::GetSize() const { return size; }
SDL_Point BOImage::GetPosition() const { return position; }
void BOImage::SetSize(SDL_Point siz) { size = siz; }
void BOImage::SetPosition(SDL_Point pos) { position = pos; }
