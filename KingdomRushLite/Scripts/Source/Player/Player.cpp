#include "../../Header/Player/Player.h"
#include "../../Header/Manager/Concrete/ProcessManager.h"

Player::Player()
{
	#pragma region StateMachine
	//优先创建被后面的指针所依赖的状态机
	stateMachine = new StateMachine();

	idleState = new PlayerIdleState(&isIdle);
	moveState = new PlayerMoveState(&isMove);
	skillState = new PlayerSkillState(&isSkill);

	//以Idle状态初始化状态机
	stateMachine->ChangeState(idleState);
	#pragma endregion

	#pragma region SkillTimer
	//在使用技能时通过ReStart函数重置，无需多次触发
	skill00Timer.SetOneShot(true);
	//具体的冷却时间在子类中被获取，故需要在子类构造函数中设置定时器的等待时间
	skill00Timer.SetTimeOutTrigger(
		[&]()
		{
			canReleaseSkill00 = true;
		}
	);

	skill01Timer.SetOneShot(true);
	skill01Timer.SetTimeOutTrigger(
		[&]()
		{
			canReleaseSkill01 = true;
		}
	);
	#pragma endregion
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
	stateMachine->OnInput(_event);
}

void Player::OnUpdate(double _delta)
{
	stateMachine->OnUpdate(_delta);

	#pragma region Skill
	if (!canReleaseSkill00)
		skill00Timer.OnUpdate(_delta);
	if (!canReleaseSkill01)
		skill01Timer.OnUpdate(_delta);
	#pragma endregion

	#pragma region Facing
	//不能判断等于0，不然可能导致静止的时候突兀转向某个方向
	if (yInput < 0) facingDir = FacingDir::Up;
	else if (yInput > 0) facingDir = FacingDir::Down;
	if (xInput < 0) facingDir = FacingDir::Left;
	else if (xInput > 0) facingDir = FacingDir::Right;
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
}

void Player::OnRender(SDL_Renderer* _renderer)
{
	stateMachine->OnRender(_renderer);
}

void Player::HandleMovementInput(const SDL_Event& _event)
{
	//移动的按键控制
	switch (_event.type)
	{
	case SDL_KEYDOWN:
		switch (_event.key.keysym.sym)
		{
		case SDLK_w:
			yInput = -1;
			break;
		case SDLK_s:
			yInput = 1;
			break;
		case SDLK_a:
			xInput = -1;
			break;
		case SDLK_d:
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
			yInput = 0;
			break;
		case SDLK_s:
			yInput = 0;
			break;
		case SDLK_a:
			xInput = 0;
			break;
		case SDLK_d:
			xInput = 0;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void Player::HandleSkillInput(const SDL_Event& _event)
{
	//技能的按键控制逻辑
	switch (_event.type)
	{
	case SDL_KEYDOWN:
		switch (_event.key.keysym.sym)
		{
		case SDLK_1:
			TryReleaseSkill00();
			break;
		case SDLK_2:
			TryReleaseSkill01();
			break;
		}
		break;
	default:
		break;
	}
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

double Player::GetSkill00CooldownRatio() const
{
	double _ratio = skill00Timer.GetPassTime() / skill00Cooldown;
	if (_ratio <= 0.01 || _ratio > 1)
		_ratio = 1;
	return _ratio;

	//把Timer::OnUpdate函数的逻辑中的passTime-=waitTime使得下面的代码会在技能恢复冷却后，_ratio变为某个>0但是很小的数
	//若改为passTime=0则可以解决该问题，但这会导致WaveManager中波次生成产生莫名其妙的混乱，暂时不明原因
	//double _ratio = skill00Timer.GetPassTime() / skill00Cooldown;
	//if (_ratio <= 0 || _ratio > 1)
	//	_ratio = 1;
	//return _ratio;
}

double Player::GetSkill01CooldownRatio() const
{
	double _ratio = skill01Timer.GetPassTime() / skill01Cooldown;
	if (_ratio <= 0.01 || _ratio > 1)
		_ratio = 1;
	return _ratio;
}