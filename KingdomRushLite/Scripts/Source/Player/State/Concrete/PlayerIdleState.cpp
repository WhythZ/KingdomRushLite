#include "../../../../Header/Player/State/Concrete/PlayerIdleState.h"
#include "../../../../Header/Manager/Concrete/PlayerManager.h"

PlayerIdleState::PlayerIdleState(bool* _flag) :PlayerState::PlayerState(_flag)
{
}

void PlayerIdleState::OnBegin()
{
	PlayerState::OnBegin();
	//std::cout << "PlayerIdleState::OnBegin()\n";
}

void PlayerIdleState::OnInput(const SDL_Event& _event)
{
	static Player* _player = PlayerManager::Instance()->player;
	
	_player->HandleMovementInput(_event);
	_player->HandleSkillInput(_event);
}

void PlayerIdleState::OnUpdate(double _delta)
{
	PlayerState::OnUpdate(_delta);

	//归零速度以刹车
	static Player* _player = PlayerManager::Instance()->player;
	_player->SetVelocity({ 0,0 });

	//更新到移动状态
	if (_player->xInput != 0 || _player->yInput != 0)
	{
		if (_player->stateMachine->GetCurrentState() != _player->moveState)
			_player->stateMachine->ChangeState(_player->moveState);
	}
}

void PlayerIdleState::OnRender(SDL_Renderer* _renderer)
{
	PlayerState::OnRender(_renderer);
}

void PlayerIdleState::OnEnd()
{
	PlayerState::OnEnd();
	//std::cout << "PlayerIdleState::OnEnd()\n";
}
