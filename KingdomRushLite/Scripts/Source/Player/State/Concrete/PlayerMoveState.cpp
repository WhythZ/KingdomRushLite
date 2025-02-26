#include "../../../../Header/Player/State/Concrete/PlayerMoveState.h"
#include "../../../../Header/Manager/Concrete/PlayerManager.h"

PlayerMoveState::PlayerMoveState() :PlayerState()
{
}

void PlayerMoveState::OnBegin()
{
	std::cout << "PlayerMoveState::OnBegin()\n";
}

void PlayerMoveState::OnUpdate(double _delta)
{
	PlayerState::OnUpdate(_delta);

	static Player* _player = PlayerManager::Instance()->player;

	//设定速度向量
	_player->UpdateVelocity(_player->xInput * _player->GetSpeed(), _player->yInput * _player->GetSpeed());
}

void PlayerMoveState::OnRender(SDL_Renderer* _renderer)
{
	PlayerState::OnRender(_renderer);
}

void PlayerMoveState::OnEnd()
{
	std::cout << "PlayerMoveState::OnEnd()\n";
}
