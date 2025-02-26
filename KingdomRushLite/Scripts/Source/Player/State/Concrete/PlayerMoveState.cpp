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

	//设定速度向量，注意归一化输入方向
	static Vector2 _direction = { _player->xInput, _player->yInput };
	_direction = _direction.Normalized();
	_player->UpdateVelocity(_direction * _player->GetSpeed());
}

void PlayerMoveState::OnRender(SDL_Renderer* _renderer)
{
	PlayerState::OnRender(_renderer);
}

void PlayerMoveState::OnEnd()
{
	std::cout << "PlayerMoveState::OnEnd()\n";
}
