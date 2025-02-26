#include "../../Header/Player/Player.h"

Player::Player()
{
	//���ȴ����������ָ����������״̬��
	stateMachine = new StateMachine();

	idleState = new PlayerIdleState();
	moveState = new PlayerMoveState();
	skillState = new PlayerSkillState();

	//��Idle״̬��ʼ��״̬��
	stateMachine->ChangeState(idleState);
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
	position += velocity * _delta;

	//����ĸı��߼�
	if (abs(xInput) >= abs(yInput))
		facingDir = (xInput <= 0) ? FacingDir::Left : FacingDir::Right;
	else
		facingDir = (yInput <= 0) ? FacingDir::Up : FacingDir::Down;

	//״̬ת���߼�
	if (isIdle && stateMachine->GetCurrentState() != idleState)
		stateMachine->ChangeState(idleState);
	else if (isMove && stateMachine->GetCurrentState() != moveState)
		stateMachine->ChangeState(moveState);
	else if (isSkill && stateMachine->GetCurrentState() != skillState)
		stateMachine->ChangeState(skillState);

	//std::cout << "xInput=" << xInput << ",yInput=" << yInput << "\n";
}

void Player::OnRender(SDL_Renderer* _renderer)
{
	stateMachine->OnRender(_renderer);
}

void Player::UpdateVelocity(const Vector2& _velocity)
{
	velocity = _velocity;
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

double Player::GetSpeed() const
{
	return speed;
}