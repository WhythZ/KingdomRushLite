#include "../../Header/Player/Player.h"

Player::Player()
{
	//初始化动画为朝下
	animCurrent = &animIdleDown;
}

void Player::SetPosition(const Vector2& _position)
{
	position = _position;
}

void Player::OnInput(const SDL_Event& _event)
{
	//switch (_event.type)
	//{
	//case:
	//	break;
	//case:
	//	break;
	//case:
	//	break;
	//case:
	//	break;
	//default:
	//	break;
	//}
}

void Player::OnUpdate(double _delta)
{
	animCurrent->OnUpdate(_delta);
}

void Player::OnRender(SDL_Renderer* _renderer)
{
	static SDL_Point _point;
	_point.x = (int)(position.x - size.x / 2);
	_point.y = (int)(position.y - size.y / 2);
	animCurrent->OnRender(_renderer, _point);
}