#include "../../Header/Player/Player.h"
#include "../../Header/Manager/Concrete/ProcessManager.h"

Player::Player()
{
	#pragma region StateMachine
	//���ȴ����������ָ����������״̬��
	stateMachine = new StateMachine();

	idleState = new PlayerIdleState(&isIdle);
	moveState = new PlayerMoveState(&isMove);
	skillState = new PlayerSkillState(&isSkill);

	//��Idle״̬��ʼ��״̬��
	stateMachine->ChangeState(idleState);
	#pragma endregion

	#pragma region SkillTimer
	//��ʹ�ü���ʱͨ��ReStart�������ã������δ���
	skill00Timer.SetOneShot(true);
	//�������ȴʱ���������б���ȡ������Ҫ�����๹�캯�������ö�ʱ���ĵȴ�ʱ��
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

	//������ٱ�ǰ���ָ����������״̬��
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
	//�����жϵ���0����Ȼ���ܵ��¾�ֹ��ʱ��ͻأת��ĳ������
	if (yInput < 0) facingDir = FacingDir::Up;
	else if (yInput > 0) facingDir = FacingDir::Down;
	if (xInput < 0) facingDir = FacingDir::Left;
	else if (xInput > 0) facingDir = FacingDir::Right;
	#pragma endregion

	#pragma region Movement
	//�ƶ��߼�
	position += velocity * _delta;
	//std::cout << velocity << "\n";
	//std::cout << position << "\n";

	//��ֹ�ɳ���ͼ
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
	//�ƶ��İ�������
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
	//���ܵİ��������߼�
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

	//��Timer::OnUpdate�������߼��е�passTime-=waitTimeʹ������Ĵ�����ڼ��ָܻ���ȴ��_ratio��Ϊĳ��>0���Ǻ�С����
	//����ΪpassTime=0����Խ�������⣬����ᵼ��WaveManager�в������ɲ���Ī������Ļ��ң���ʱ����ԭ��
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