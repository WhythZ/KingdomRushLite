#include "../../Header/Player/Player.h"
#include "../../Header/Manager/Concrete/ProcessManager.h"

Player::Player()
{
	//优先创建被后面的指针所依赖的状态机
	stateMachine = new StateMachine();

	idleState = new PlayerIdleState();
	moveState = new PlayerMoveState();
	skillState = new PlayerSkillState();

	//以Idle状态初始化状态机
	//stateMachine->ChangeState(idleState);
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

void Player::SetVelocity(const Vector2& _velocity)
{
	velocity = _velocity;
}

void Player::OnInput(const SDL_Event& _event)
{
	#pragma region Move
	//此处注释掉的部分直接对isMove进行赋值，当连续按下多个方向键再松开时，会导致直接进入Idle状态而非保持Move状态，手感卡顿
	//switch (_event.type)
	//{
	//case SDL_KEYDOWN:
	//	switch (_event.key.keysym.sym)
	//	{
	//	case SDLK_w:
	//		isIdle = false;
	//		isMove = true;
	//		yInput = -1;
	//		break;
	//	case SDLK_s:
	//		isIdle = false;
	//		isMove = true;
	//		yInput = 1;
	//		break;
	//	case SDLK_a:
	//		isIdle = false;
	//		isMove = true;
	//		xInput = -1;
	//		break;
	//	case SDLK_d:
	//		isIdle = false;
	//		isMove = true;
	//		xInput = 1;
	//		break;
	//	default:
	//		break;
	//	}
	//	break;
	//case SDL_KEYUP:
	//	switch (_event.key.keysym.sym)
	//	{
	//	case SDLK_w:
	//		isIdle = true;
	//		isMove = false;
	//		yInput = 0;
	//		break;
	//	case SDLK_s:
	//		isIdle = true;
	//		isMove = false;
	//		yInput = 0;
	//		break;
	//	case SDLK_a:
	//		isIdle = true;
	//		isMove = false;
	//		xInput = 0;
	//		break;
	//	case SDLK_d:
	//		isIdle = true;
	//		isMove = false;
	//		xInput = 0;
	//		break;
	//	default:
	//		break;
	//	}
	//	break;
	//default:
	//	break;
	//}

	//移动的按键控制
	switch (_event.type)
	{
	case SDL_KEYDOWN:
		switch (_event.key.keysym.sym)
		{
		case SDLK_w:
			isIdle = false;
			isMoveUp = true;
			yInput = -1;
			break;
		case SDLK_s:
			isIdle = false;
			isMoveDown = true;
			yInput = 1;
			break;
		case SDLK_a:
			isIdle = false;
			isMoveLeft = true;
			xInput = -1;
			break;
		case SDLK_d:
			isIdle = false;
			isMoveRight = true;
			xInput = 1;
			break;
		default:
			break;
		}
		break;
	case SDL_KEYUP:
		switch (_event.key.keysym.sym)
		{
		case SDLK_w:
			isMoveUp = false;
			yInput = 0;
			break;
		case SDLK_s:
			isMoveDown = false;
			yInput = 0;
			break;
		case SDLK_a:
			isMoveLeft = false;
			xInput = 0;
			break;
		case SDLK_d:
			isMoveRight = false;
			xInput = 0;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	//综合得出isMove的值（这要在技能的按键监测前，因为释放技能时不能移动，防止释放技能时isMove为真）
	isMove = isMoveUp || isMoveDown || isMoveLeft || isMoveRight;
	#pragma endregion

	#pragma region Skill
	//技能的按键控制逻辑
	switch (_event.type)
	{
	case SDL_KEYDOWN:
		switch (_event.key.keysym.sym)
		{
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
		}
		break;
	default:
		break;
	}
	#pragma endregion

	#pragma region Idle
	//最终若无事发生，则进入Idle状态
	if (!isSkill && !isMove)
		isIdle = true;
	#pragma endregion
}

void Player::OnUpdate(double _delta)
{
	stateMachine->OnUpdate(_delta);

	#pragma region Facing
	//不能判断等于0，不然可能导致静止的时候突兀转向某个方向
	if (yInput > 0) facingDir = FacingDir::Down;
	if (yInput < 0) facingDir = FacingDir::Up;
	if (xInput > 0) facingDir = FacingDir::Right;
	if (xInput < 0) facingDir = FacingDir::Left;
	#pragma endregion

	#pragma region Movement
	//移动逻辑
	position += velocity * _delta;
	//std::cout << velocity << "\n";
	//std::cout << position << "\n";
	
	//防止飞出地图
	static SDL_Rect _mapRect = ProcessManager::Instance()->mapRect;
	if (position.x <= _mapRect.x) position.x = _mapRect.x;
	if (position.x >= _mapRect.x + _mapRect.w) position.x = _mapRect.x + _mapRect.w;
	if (position.y <= _mapRect.y) position.y = _mapRect.y;
	if (position.y >= _mapRect.y + _mapRect.h) position.y = _mapRect.y + _mapRect.h;
	#pragma endregion

	#pragma region State
	//状态转换逻辑，自上而下具有优先级
	if (isSkill && stateMachine->GetCurrentState() != skillState)
		stateMachine->ChangeState(skillState);
	else if (isMove && stateMachine->GetCurrentState() != moveState)
		stateMachine->ChangeState(moveState);
	else if (isIdle && stateMachine->GetCurrentState() != idleState)
		stateMachine->ChangeState(idleState);
	#pragma endregion
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

double Player::GetSpeed() const
{
	return speed;
}