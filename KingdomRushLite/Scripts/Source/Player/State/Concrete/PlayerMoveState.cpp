#include "../../../../Header/Player/State/Concrete/PlayerMoveState.h"
#include "../../../../Header/Manager/Concrete/PlayerManager.h"

PlayerMoveState::PlayerMoveState() :PlayerState()
{
}

void PlayerMoveState::OnBegin()
{
	//std::cout << "PlayerMoveState::OnBegin()\n";
}

void PlayerMoveState::OnUpdate(double _delta)
{
	PlayerState::OnUpdate(_delta);

	//依照方向更新速度
	static PlayerManager* _pm = PlayerManager::Instance();
	Vector2 _direction = { _pm->player->xInput, _pm->player->yInput };
	_pm->player->SetVelocity(_direction.Normalized() * _pm->player->GetSpeed());
}

void PlayerMoveState::OnRender(SDL_Renderer* _renderer)
{
	PlayerState::OnRender(_renderer);
}

void PlayerMoveState::OnEnd()
{
	//std::cout << "PlayerMoveState::OnEnd()\n";
}
