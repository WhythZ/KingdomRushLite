#include "../../../../Header/Player/State/Concrete/PlayerMoveState.h"

PlayerMoveState::PlayerMoveState(Player* _player) :PlayerState(_player)
{
}

void PlayerMoveState::OnBegin()
{
	std::cout << "PlayerMoveState::OnBegin()\n";
}

void PlayerMoveState::OnUpdate(double _delta)
{
	PlayerState::OnUpdate(_delta);

	//设定速度向量
	player->UpdateVelocity(player->xInput * speed, player->yInput * speed);
}

void PlayerMoveState::OnRender(SDL_Renderer* _renderer)
{
	PlayerState::OnRender(_renderer);
}

void PlayerMoveState::OnEnd()
{
	std::cout << "PlayerMoveState::OnEnd()\n";
}
