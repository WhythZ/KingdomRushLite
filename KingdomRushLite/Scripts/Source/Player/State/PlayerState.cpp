#include "../../../Header/Player/State/PlayerState.h"
#include "../../../Header/Manager/Concrete/ConfigManager.h"
#include "../../../Header/Manager/Concrete/PlayerManager.h"

PlayerState::PlayerState(bool* _flag) :State::State(_flag)
{
	//�����µķ����ʼ��
	animCurrent = &animDown;
}

void PlayerState::OnBegin()
{
	State::OnBegin();
}

void PlayerState::OnUpdate(double _delta)
{
	//����ǰ����ָ��ָ���Ӧ����Ķ���
	switch (PlayerManager::Instance()->player->facingDir)
	{
	case FacingDir::Up:
		animCurrent = &animUp;
		break;
	case FacingDir::Down:
		animCurrent = &animDown;
		break;
	case FacingDir::Left:
		animCurrent = &animLeft;
		break;
	case FacingDir::Right:
		animCurrent = &animRight;
		break;
	default:
		break;
	}

	animCurrent->OnUpdate(_delta);
}

void PlayerState::OnRender(SDL_Renderer* _renderer)
{
	static Player* _player = PlayerManager::Instance()->player;

	static SDL_Point _point;
	_point.x = (int)(_player->GetPosition().x - _player->GetSize().x / 2);
	_point.y = (int)(_player->GetPosition().y - _player->GetSize().y / 2);
	animCurrent->OnRender(_renderer, _point);
}

void PlayerState::OnEnd()
{
	State::OnEnd();
}