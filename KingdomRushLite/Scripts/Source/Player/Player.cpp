#include "../../Header/Player/Player.h"

Player::Player()
{
	//优先创建被后面的指针所依赖的状态机
	stateMachine = new StateMachine();

	idleState = new PlayerIdleState();
	moveState = new PlayerMoveState();
	skillState = new PlayerSkillState();

	//以Idle状态初始化状态机
	stateMachine->ChangeState(idleState);
}

Player::~Player()
{
	delete idleState;
	delete moveState;
	delete skillState;

	//最后销毁被前面的指针所依赖的状态机
	delete stateMachine;
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
			isIdle = false;
			isMove = true;
			yInput = -1;
			break;
		case SDLK_s:
			isIdle = false;
			isMove = true;
			yInput = 1;
			break;
		case SDLK_a:
			isIdle = false;
			isMove = true;
			xInput = -1;
			break;
		case SDLK_d:
			isIdle = false;
			isMove = true;
			xInput = 1;
			break;
		//技能的按键控制逻辑
		case SDLK_1:
			isIdle = false;
			isMove = false;
			isSkill = true;
			ReleaseSkill00();
			break;
		case SDLK_2:
			isIdle = false;
			isMove = false;
			isSkill = true;
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
			isIdle = true;
			isMove = false;
			yInput = 0;
			break;
		case SDLK_s:
			isIdle = true;
			isMove = false;
			yInput = 0;
			break;
		case SDLK_a:
			isIdle = true;
			isMove = false;
			xInput = 0;
			break;
		case SDLK_d:
			isIdle = true;
			isMove = false;
			xInput = 0;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	//std::cout << "xInput=" << xInput << ",yInput=" << yInput << "\n";
}

void Player::OnUpdate(double _delta)
{
	stateMachine->OnUpdate(_delta);

	//移动逻辑
	Vector2 _direction = { xInput, yInput };
	velocity = _direction.Normalized() * speed;
	position += velocity * _delta;

	//朝向的改变逻辑，此处不能将==纳入判断，否则会导致玩家朝向总是归正到某一个轴上
	if (abs(xInput) > abs(yInput))
		facingDir = (xInput <= 0) ? FacingDir::Left : FacingDir::Right;
	else if (abs(xInput) < abs(yInput))
		facingDir = (yInput <= 0) ? FacingDir::Up : FacingDir::Down;

	//状态转换逻辑
	if (isIdle && stateMachine->GetCurrentState() != idleState)
		stateMachine->ChangeState(idleState);
	else if (isMove && stateMachine->GetCurrentState() != moveState)
		stateMachine->ChangeState(moveState);
	else if (isSkill && stateMachine->GetCurrentState() != skillState)
		stateMachine->ChangeState(skillState);
}

void Player::OnRender(SDL_Renderer* _renderer)
{
	stateMachine->OnRender(_renderer);
}

PlayerType Player::GetType() const
{
	return type;
}

const Vector2& Player::GetSize() const
{
	return size;
}

const Vector2& Player::GetPosition() const
{
	return position;
}