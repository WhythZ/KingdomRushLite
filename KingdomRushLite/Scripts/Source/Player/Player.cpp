#include "../../Header/Player/Player.h"
#include "../../Header/Manager/Concrete/ProcessManager.h"

Player::Player()
{
	//���ȴ����������ָ����������״̬��
	stateMachine = new StateMachine();

	idleState = new PlayerIdleState();
	moveState = new PlayerMoveState();
	skillState = new PlayerSkillState();

	//��Idle״̬��ʼ��״̬��
	//stateMachine->ChangeState(idleState);
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
	#pragma region Move
	//�˴�ע�͵��Ĳ���ֱ�Ӷ�isMove���и�ֵ�����������¶����������ɿ�ʱ���ᵼ��ֱ�ӽ���Idle״̬���Ǳ���Move״̬���ָп���
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

	//�ƶ��İ�������
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
	//�ۺϵó�isMove��ֵ����Ҫ�ڼ��ܵİ������ǰ����Ϊ�ͷż���ʱ�����ƶ�����ֹ�ͷż���ʱisMoveΪ�棩
	isMove = isMoveUp || isMoveDown || isMoveLeft || isMoveRight;
	#pragma endregion

	#pragma region Skill
	//���ܵİ��������߼�
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
	//���������·����������Idle״̬
	if (!isSkill && !isMove)
		isIdle = true;
	#pragma endregion
}

void Player::OnUpdate(double _delta)
{
	stateMachine->OnUpdate(_delta);

	#pragma region Facing
	//�����жϵ���0����Ȼ���ܵ��¾�ֹ��ʱ��ͻأת��ĳ������
	if (yInput > 0) facingDir = FacingDir::Down;
	if (yInput < 0) facingDir = FacingDir::Up;
	if (xInput > 0) facingDir = FacingDir::Right;
	if (xInput < 0) facingDir = FacingDir::Left;
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

	#pragma region State
	//״̬ת���߼������϶��¾������ȼ�
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