#include "../../../Header/Player/State/PlayerState.h"
#include "../../../Header/Manager/Concrete/ConfigManager.h"

PlayerState::PlayerState(Player* _player)
{
	player = _player;

	//��ȡ��Ӧ������ҵ��ٶ�
	switch (player->GetType())
	{
	case PlayerType::Dragon:
		speed = ConfigManager::Instance()->playerDragonPrefab.speed;
		break;
	default:
		speed = 0;
		break;
	}
}

void PlayerState::OnUpdate(double _delta)
{
	//����ǰ����ָ��ָ���Ӧ����Ķ���
	switch (player->facingDir)
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
	static SDL_Point _point;
	_point.x = (int)(player->GetPosition().x - player->GetSize().x / 2);
	_point.y = (int)(player->GetPosition().y - player->GetSize().y / 2);
	animCurrent->OnRender(_renderer, _point);
}