#include "../../Header/Player/Player.h"

Player::Player()
{
	//���ȴ����������ָ����������״̬��
	stateMachine = new StateMachine();

	idleUpState = new PlayerIdleUpState(this);
	idleDownState = new PlayerIdleDownState(this);
	idleLeftState = new PlayerIdleLeftState(this);
	idleRightState = new PlayerIdleRightState(this);
	
	moveUpState = new PlayerMoveUpState(this);
	moveDownState = new PlayerMoveDownState(this);
	moveLeftState = new PlayerMoveLeftState(this);
	moveRightState = new PlayerMoveRightState(this);

	skillUpState = new PlayerSkillUpState(this);
	skillDownState = new PlayerSkillDownState(this);
	skillLeftState = new PlayerSkillLeftState(this);
	skillRightState = new PlayerSkillRightState(this);

	//�����µ�Idle״̬��ʼ��
	stateMachine->ChangeState(idleDownState);
}

Player::~Player()
{
	delete idleUpState;
	delete idleDownState;
	delete idleLeftState;
	delete idleRightState;

	delete moveUpState;
	delete moveDownState;
	delete moveLeftState;
	delete moveRightState;

	delete skillUpState;
	delete skillDownState;
	delete skillLeftState;
	delete skillRightState;

	//������ٱ�ǰ���ָ����������״̬��
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
		//�ƶ��İ������Ʋ����߼�
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
		//���ܵİ��������߼�
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
		//�ƶ��İ������Ʋ����߼�
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
}

void Player::OnUpdate(double _delta)
{
	stateMachine->OnUpdate(_delta);

	//�ƶ��߼�
	position += velocity.Normalized() * _delta;

	//����ĸı��߼�
	if (abs(xInput) >= abs(yInput))
		facingDir = (xInput <= 0) ? FacingDir::Left : FacingDir::Right;
	else
		facingDir = (yInput <= 0) ? FacingDir::Up : FacingDir::Down;

	//״̬ת���߼�
	if (isIdle)
		stateMachine->ChangeState(idleState);
	else if (isMove)
		stateMachine->ChangeState(moveState);
	else if (isSkill)
		stateMachine->ChangeState(skillState);
}

void Player::OnRender(SDL_Renderer* _renderer)
{
	stateMachine->OnRender(_renderer);
}

void Player::UpdateVelocity(double _x, double _y)
{
	velocity.x = _x;
	velocity.y = _y;
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