#include "../../../../Header/Player/State/Concrete/PlayerMoveState.h"
#include "../../../../Header/Manager/Concrete/PlayerManager.h"

PlayerMoveState::PlayerMoveState(bool* _flag) :PlayerState(_flag)
{
}

void PlayerMoveState::OnBegin()
{
	PlayerState::OnBegin();
	//std::cout << "PlayerMoveState::OnBegin()\n";
}

void PlayerMoveState::OnInput(const SDL_Event& _event)
{
	static Player* _player = PlayerManager::Instance()->player;
	
	_player->HandleMovementInput(_event);
	_player->HandleSkillInput(_event);
}

void PlayerMoveState::OnUpdate(double _delta)
{
	PlayerState::OnUpdate(_delta);

	//���շ�������ٶ�
	static Player* _player = PlayerManager::Instance()->player;
	Vector2 _direction = { _player->xInput, _player->yInput };
	_player->SetVelocity(_direction.Normalized() * _player->GetSpeed());

	//���µ�����״̬
	if (_player->xInput == 0 && _player->yInput == 0)
	{
		if (_player->stateMachine->GetCurrentState() != _player->idleState)
			_player->stateMachine->ChangeState(_player->idleState);
	}
}

void PlayerMoveState::OnRender(SDL_Renderer* _renderer)
{
	PlayerState::OnRender(_renderer);
}

void PlayerMoveState::OnEnd()
{
	PlayerState::OnEnd();
	//std::cout << "PlayerMoveState::OnEnd()\n";
}
