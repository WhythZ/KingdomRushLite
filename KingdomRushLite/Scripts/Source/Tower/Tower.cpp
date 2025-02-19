#include "../../Header/Tower/Tower.h"
#include "../../Header/Manager/Concrete/EnemyManager.h"
#include "../../Header/Manager/Concrete/BulletManager.h"
#include "../../Header/Manager/Concrete/TowerManager.h"
#include <iostream>

Tower::Tower()
{
	#pragma region Animation
	//��ʼ�������ĸ������ԣ��������������������ã�
	animIdleUp.SetLoop(true);
	animIdleDown.SetLoop(true);
	animIdleLeft.SetLoop(true);
	animIdleRight.SetLoop(true);
	animFireUp.SetLoop(false);
	animFireDown.SetLoop(false);
	animFireLeft.SetLoop(false);
	animFireRight.SetLoop(false);

	animIdleUp.SetFrameInterval(ANIMATION_LARGER_FRAME_INTERVAL);
	animIdleDown.SetFrameInterval(ANIMATION_LARGER_FRAME_INTERVAL);
	animIdleLeft.SetFrameInterval(ANIMATION_LARGER_FRAME_INTERVAL);
	animIdleRight.SetFrameInterval(ANIMATION_LARGER_FRAME_INTERVAL);
	animFireUp.SetFrameInterval(ANIMATION_LARGER_FRAME_INTERVAL);
	animFireDown.SetFrameInterval(ANIMATION_LARGER_FRAME_INTERVAL);
	animFireLeft.SetFrameInterval(ANIMATION_LARGER_FRAME_INTERVAL);
	animFireRight.SetFrameInterval(ANIMATION_LARGER_FRAME_INTERVAL);

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

	//������ڿɿ����״̬����׼������������ݲ�����
	if (canFire)
	{
		//�����ӵ����������ݣ�����FindTargetEnemy()ǰ���£���ú�����ʹ�������а뾶����
		static TowerManager* _tm = TowerManager::Instance();
		fireCooldown = _tm->GetFireCooldownOf(type);
		fireRadius = _tm->GetFireRadiusOf(type);
		bulletDamage = _tm->GetBulletDamageOf(type);
		//std::cout << "FireRadius=" << fireRadius << "\n";

		//�����ʱδ�ҵ��ɹ���Ŀ�꣬��������к��������˷���Դ
		Enemy* _target = FindTargetEnemy();
		if (_target == nullptr)
			return;

		//��Ŀ����˿���
		OnFireBullet(_target);

		//���ÿɿ���״̬��Ȼ�����ü�ʱ��������ȴ��canFire���ص��������¸�ֵΪtrue
		canFire = false;
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

void Tower::OnFireBullet(Enemy* _target)
{
	//����ĳ���
	Vector2 _direction = (_target->GetPosition() - this->position).Normalized();

	#pragma region UpdateTowerAnimation
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
	double _maxProcess = -1;
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
