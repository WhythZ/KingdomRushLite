#include "../../../../Header/Player/State/Concrete/PlayerSkillState.h"

PlayerSkillState::PlayerSkillState(Player* _player) :PlayerState::PlayerState(_player)
{
}

void PlayerSkillState::OnBegin()
{
	std::cout << "PlayerSkillState::OnBegin()\n";
}

void PlayerSkillState::OnUpdate(double _delta)
{
	PlayerState::OnUpdate(_delta);
}

void PlayerSkillState::OnRender(SDL_Renderer* _renderer)
{
	PlayerState::OnRender(_renderer);
}

void PlayerSkillState::OnEnd()
{
	std::cout << "PlayerSkillState::OnEnd()\n";
}