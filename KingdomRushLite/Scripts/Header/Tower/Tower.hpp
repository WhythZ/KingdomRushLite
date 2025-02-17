#ifndef _TOWER_HPP_
#define _TOWER_HPP_

#include "TowerType.hpp"
#include "../Math/Vector2.hpp"
#include "../Math/Timer.hpp"
#include "../Math/FacingDir.hpp"
#include "../Animation/Animation.hpp"
#include "../Manager/Concrete/BulletManager.hpp"

class Tower
{
protected:
	TowerType type = TowerType::None;          //������������

	#pragma region Transform
	Vector2 size;                              //������Ⱦ����ײ�ĳߴ�
	#pragma endregion

	#pragma region Animation
	Animation animIdleUp;              //�������ϵĶ���
	Animation animIdleDown;            //�������µĶ���
	Animation animIdleLeft;            //��������Ķ���
	Animation animIdleRight;           //�������ҵĶ���

	Animation animFireUp;              //�������ϵĶ���
	Animation animFireDown;            //�������µĶ���
	Animation animFireLeft;            //��������Ķ���
	Animation animFireRight;           //�������ҵĶ���
	#pragma endregion

	#pragma region FireProperties
	BulletType bulletType = BulletType::None;  //������ӵ�����
	float fireSpeed = 0;                       //�����ӵ����ٶ�
	#pragma endregion

private:
	Vector2 position;                          //����������λ��
	
	Animation* animCurrent = &animIdleDown;    //������Ĭ�϶���
	FacingDir direction = FacingDir::Down;     //������Ĭ�϶����ĳ���

	Timer fireTimer;                           //�����ӵ��������Ķ�ʱ��
	bool canFire = true;                       //�����Ƿ���Է����ӵ�

public:
	Tower();
	~Tower() = default;
	void SetPosition(const Vector2&);          //���÷�������λ��

	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	const Vector2& GetSize() const;            //��ȡ�������ĳߴ�
	const Vector2& GetPosition() const;        //��ȡ��������λ��

private:
	void UpdateFacingDir();                    //���ճ�����µ�ǰ����
	void OnFire();                             //�����ӵ����߼�
};

Tower::Tower()
{
	#pragma region Animation
	//��ʼ�����������ԣ����������������ã�
	animIdleUp.SetLoop(true);
	animIdleDown.SetLoop(true);
	animIdleLeft.SetLoop(true);
	animIdleRight.SetLoop(true);
	animFireUp.SetLoop(false);
	animFireDown.SetLoop(false);
	animFireLeft.SetLoop(false);
	animFireRight.SetLoop(false);

	//ֻ���ڷ��������һ�ι����󣬲��ܸ��·���������
	animFireUp.SetOnAnimFinished(
		[&]()
		{
			UpdateFacingDir();
		}
	);
	animFireDown.SetOnAnimFinished(
		[&]()
		{
			UpdateFacingDir();
		}
	);
	animFireLeft.SetOnAnimFinished(
		[&]()
		{
			UpdateFacingDir();
		}
	);
	animFireRight.SetOnAnimFinished(
		[&]()
		{
			UpdateFacingDir();
		}
	);
	#pragma endregion

	//��ʼ������ʱ��
	fireTimer.SetTimeOutTrigger(
		[&]()
		{
			//�ָ����ɷ����ӵ���״̬
			canFire = true;
		}
	);
}

void Tower::SetPosition(const Vector2& _position)
{
	position = _position;
}

void Tower::OnUpdate(double _delta)
{
	fireTimer.OnUpdate(_delta);
	animCurrent->OnUpdate(_delta);

	//������Կ�������ÿ���
	if (canFire)
		OnFire();
}

void Tower::OnRender(SDL_Renderer* _renderer)
{
	//��������ͼ�����ϽǶ�������
	static SDL_Point _point;
	_point.x = (int)(position.x - size.x / 2);
	_point.y = (int)(position.y - size.y / 2);
	//���µ�ǰ����
	animCurrent->OnRender(_renderer, _point);
}

const Vector2& Tower::GetSize() const
{
	return size;
}

const Vector2& Tower::GetPosition() const
{
	return position;
}

void Tower::UpdateFacingDir()
{
	//�˴�ʹ�ö�Ӧ���������״̬�������и���
	switch (direction)
	{
	case FacingDir::Up:
		animCurrent = &animIdleUp;
		break;
	case FacingDir::Down:
		animCurrent = &animIdleDown;
		break;
	case FacingDir::Left:
		animCurrent = &animIdleLeft;
		break;
	case FacingDir::Right:
		animCurrent = &animIdleRight;
		break;
	default:
		break;
	}
}

void Tower::OnFire()
{

}

#endif
