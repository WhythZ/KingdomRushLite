#include "../../Header/Player/Player.h"

Player::Player()
{

}

Player::~Player()
{

}

void Player::SetPosition(const Vector2& _position)
{
	position = _position;
}

void Player::OnInput(const SDL_Event& _event)
{
	switch (_event.type)
	{
	case SDL_KEYDOWN:
		switch (_event.key.keysym.sym)
		{
		//移动的按键控制部分逻辑
		case SDLK_w:
			isMovingUp = true;
			break;
		case SDLK_a:
			isMovingLeft = true;
			break;
		case SDLK_s:
			isMovingDown = true;
			break;
		case SDLK_d:
			isMovingRight = true;
			break;
		//技能的按键控制逻辑
		case SDLK_1:
			ReleaseSkill00();
			break;
		case SDLK_2:
			ReleaseSkill01();
			break;
		default:
			break;
		}
		break;
	case SDL_KEYUP:
		switch (_event.key.keysym.sym)
		{
		//移动的按键控制部分逻辑
		case SDLK_w:
			isMovingUp = false;
			break;
		case SDLK_a:
			isMovingLeft = false;
			break;
		case SDLK_s:
			isMovingDown = false;
			break;
		case SDLK_d:
			isMovingRight = false;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void Player::OnUpdate(double _delta)
{
}

void Player::OnRender(SDL_Renderer* _renderer)
{

}