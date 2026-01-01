#include "engine/Game.h"
#include "engine/Util.h"
#include "scene/SAbout.h"
#include "scene/SPatch.h"
#include "scene/SReadme.h"
#include <SDL3/SDL_main.h>

int main(int argc, char **argv) {
  Util::LoadAssets();
  Game::Init();
  Game::CreateScene(SceneType::SPatch, new SPatch);
  Game::CreateScene(SceneType::SReadme, new SReadme);
  Game::CreateScene(SceneType::SAbout, new SAbout);
  Game::SwitchTo(SceneType::SReadme);
  Game::Run();
  return 0;
}