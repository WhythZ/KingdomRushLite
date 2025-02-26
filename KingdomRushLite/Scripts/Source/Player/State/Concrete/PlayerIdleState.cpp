#include "../../../../Header/Player/State/Concrete/PlayerIdleState.h"
#include "../../../../Header/Manager/Concrete/PlayerManager.h"

PlayerIdleState::PlayerIdleState() :PlayerState::PlayerState()
{
}

void PlayerIdleState::OnBegin()
{
	std::cout << "PlayerIdleState::OnBegin()\n";
}

void PlayerIdleState::OnUpdate(double _delta)
{
	PlayerState::OnUpdate(_delta);
}

void PlayerIdleState::OnRender(SDL_Renderer* _renderer)
{
	PlayerState::OnRender(_renderer);
}

void PlayerIdleState::OnEnd()
{
	std::cout << "PlayerIdleState::OnEnd()\n";
}
