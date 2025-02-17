#ifndef _TOWER_HPP_
#define _TOWER_HPP_

#include "TowerType.hpp"
#include "../Math/Vector2.hpp"
#include "../Math/Timer.hpp"
#include "../Math/FacingDir.hpp"
#include "../Animation/Animation.hpp"
#include "../Manager/Concrete/BulletManager.hpp"
#include "../Manager/Concrete/EnemyManager.hpp"

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

	#pragma region Properties
	float fireCooldown = 0;                    //ÿ�η���ļ��
	float fireRadius = 0;                      //������Χ�뾶
	BulletType bulletType = BulletType::None;  //�ӵ�������
	float bulletDamage = 0;                    //�ӵ����˺�
	float bulletSpeed = 0;                     //�ӵ����ٶ�
	#pragma endregion

private:
	Vector2 position;                          //����������λ��
	
	Animation* animCurrent = &animIdleDown;    //������Ĭ�϶���
	FacingDir facingDir = FacingDir::Down;     //������Ĭ�϶����ĳ���

	bool canFire = true;                       //�����Ƿ���Է����ӵ�
	Timer fireTimer;                           //�����ӵ��������Ķ�ʱ��

public:
	Tower();
	~Tower() = default;
	void SetPosition(const Vector2&);          //���÷�������λ��

	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	const Vector2& GetSize() const;            //��ȡ�������ĳߴ�
	const Vector2& GetPosition() const;        //��ȡ��������λ��
		
protected:
	void virtual UpdateBulletData() = 0;       //�����ӵ����ԵĴ��麯��
	void virtual OnFireBullet();               //�ӵ�����ʱ���߼�

private:
	void UpdateFacingDir(const Vector2&);      //���·���������
	void UpdateIdleAnim();                     //���ճ�����µ�ǰ����Ϊ��Ӧ���ö���
	void UpdateFireAnim();                     //���ճ�����µ�ǰ����Ϊ��Ӧ���𶯻�
	Enemy* FindTargetEnemy();                  //�ӵ����е��߼�
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

	//ֻ�������һ�ο���󣬲��ܸ��µ�ǰ����Ϊ��Ӧ��������ö������൱�ڴӶ�Ӧ���򿪻𶯻�ת������Ӧ�������ö�����
	animFireUp.SetOnAnimFinished(
		[&]()
		{
			UpdateIdleAnim();
		}
	);
	animFireDown.SetOnAnimFinished(
		[&]()
		{
			UpdateIdleAnim();
		}
	);
	animFireLeft.SetOnAnimFinished(
		[&]()
		{
			UpdateIdleAnim();
		}
	);
	animFireRight.SetOnAnimFinished(
		[&]()
		{
			UpdateIdleAnim();
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

	//�����ӵ����������ݣ�����λ����˲ʱ�ٶ�����
	UpdateBulletData();

	//������ڿɿ����״̬����׼�����ڿ��������
	if (canFire)
	{
		//�����ӵ������߼�
		OnFireBullet();

		//���ÿɿ���״̬
		canFire = false;
		//���ü�ʱ�����ȴ�������ȴ��canFire���ص��������¸�ֵΪtrue
		fireTimer.SetWaitTime(fireCooldown);
		fireTimer.Restart();
	}
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

void Tower::OnFireBullet()
{
	//�����ʱδ�ҵ��ɹ���Ŀ�꣬��������к��������˷���Դ
	Enemy* _target = FindTargetEnemy();
	if (!_target)
		return;

	//����ĳ���
	Vector2 _direction = (_target->GetPosition() - this->position).Normalized();

	#pragma region UpdateAnimation
	//���ݿ�������·���������
	UpdateFacingDir(_direction);
	//Ȼ���ճ�����µ�ǰ����Ϊ��Ӧ����Ŀ��𶯻�
	UpdateFireAnim();
	#pragma endregion

	#pragma region InstantiateBullet
	//���ݿ�����ʵ�����ӵ���������ٶ������ǵó�����Ƭ��С�������ɱ����������ʵĶ����������ģ�
	BulletManager::Instance()->FireBullet(bulletType, this->position, _direction * bulletSpeed * TILE_SIZE, bulletDamage);
	#pragma endregion
}

void Tower::UpdateFacingDir(const Vector2& _direction)
{
	//���ж���ˮƽ������ֱ
	bool _isHorizontal = abs(_direction.x) >= abs(_direction.y);

	//��ѡȡ������������᷽��
	if (_isHorizontal)
		facingDir = (_direction.x > 0) ? FacingDir::Right : FacingDir::Left;
	else
		facingDir = (_direction.y < 0) ? FacingDir::Up : FacingDir::Down;
}

void Tower::UpdateIdleAnim()
{
	switch (facingDir)
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

void Tower::UpdateFireAnim()
{
	switch (facingDir)
	{
	case FacingDir::Up:
		animCurrent = &animFireUp;
		break;
	case FacingDir::Down:
		animCurrent = &animFireDown;
		break;
	case FacingDir::Left:
		animCurrent = &animFireLeft;
		break;
	case FacingDir::Right:
		animCurrent = &animFireRight;
		break;
	default:
		break;
	}
}

Enemy* Tower::FindTargetEnemy()
{
	Enemy* _target = nullptr;
	EnemyManager::EnemyList _enemyList = EnemyManager::Instance()->GetEnemyList();

	//�ڹ�����Χ�ڵĵ����У�Ӧ�����ȹ���������ſڵ��Ǹ�
	double _maxProcess = 0;
	for (Enemy* _enemy : _enemyList)
	{
		//ȷ�������ڹ����뾶��Χ��
		if ((_enemy->GetPosition() - this->position).Length() <= fireRadius)
		{
			//���ϱ������������ɶȣ����յõ�·����ɶ����Ĺ���������ſ����
			double _newProcess = _enemy->GetRouteProcess();
			if (_newProcess > _maxProcess)
			{
				_target = _enemy;
				_maxProcess = _newProcess;
			}
		}
	}

	//����Ŀ�����ָ��
	return _target;
}

#endif
