#ifndef O_CHECK_TOOL
#define O_CHECK_TOOL
#include "../engine/GameObject.h"
#include "../engine/GameConfig.h"

class OPatchValue : GameObject {
public:
  enum class PatchValueState { NotReady, Ready, Failed };
  OPatchValue();
  void Draw() override {}
  void Update(float dt) override {};
  PatchValueState GetState();
  bool Download();
  std::string GetValue(std::string key);

private:
  PatchValueState currentState = PatchValueState::NotReady;
  GameConfig config;
};
#endif