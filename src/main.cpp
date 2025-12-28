#include "engine/GameManager.h"
#include "scene/SInstaller.h"
#include <SDL3/SDL_main.h>

int main(int argc, char **argv) {
  GameManager *manager = GameManager::Get();
  manager->Init();
  manager->SetScene(new SInstaller());
  manager->Run();
  return 0;
}