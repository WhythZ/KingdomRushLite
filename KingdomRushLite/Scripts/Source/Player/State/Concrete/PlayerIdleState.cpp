#include "../../../../Header/Player/State/Concrete/PlayerIdleState.h"

PlayerIdleState::PlayerIdleState(Player* _player) :PlayerState::PlayerState(_player)
{
}

void PlayerIdleState::OnBegin()
{
	std::cout << "PlayerIdleState::OnBegin()\n";
}

void PlayerIdleState::OnUpdate(double _delta)
{
	PlayerState::OnUpdate(_delta);

	//��ֹ��ԭ��
	player->UpdateVelocity(0, 0);
}

void PlayerIdleState::OnRender(SDL_Renderer* _renderer)
{
	PlayerState::OnRender(_renderer);
}

void PlayerIdleState::OnEnd()
{
	std::cout << "PlayerIdleState::OnEnd()\n";
}
