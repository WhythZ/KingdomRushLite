#include "../../Header/Tower/Tower.h"
#include "../../Header/Manager/Concrete/EnemyManager.h"
#include "../../Header/Manager/Concrete/BulletManager.h"
#include "../../Header/Manager/Concrete/TowerManager.h"
#include <iostream>

Tower::Tower()
{
	#pragma region Animation
	//初始化动画的各种属性（具体纹理在子类中设置）
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

	//只有在完成一次开火后，才能更新当前动画为对应朝向的闲置动画（相当于从对应方向开火动画转换到对应方向闲置动画）
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

	//初始化开火定时器
	fireTimer.SetTimeOutTrigger(
		[&]()
		{
			//恢复到可发射子弹的状态
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

	//如果处于可开火的状态，则准备更新相关数据并开火
	if (canFire)
	{
		//更新子弹的属性数据，需在FindTargetEnemy()前更新，因该函数内使用了索敌半径数据
		static TowerManager* _tm = TowerManager::Instance();
		fireCooldown = _tm->GetFireCooldownOf(type);
		fireRadius = _tm->GetFireRadiusOf(type);
		bulletDamage = _tm->GetBulletDamageOf(type);
		//std::cout << "FireRadius=" << fireRadius << "\n";

		//如果暂时未找到可攻击目标，则无需进行后续更新浪费资源
		Enemy* _target = FindTargetEnemy();
		if (_target == nullptr)
			return;

		//向目标敌人开火
		OnFireBullet(_target);

		//重置可开火状态，然后重置计时器，经冷却后canFire被回调函数重新赋值为true
		canFire = false;
		fireTimer.SetWaitTime(fireCooldown);
		fireTimer.Restart();
	}
}

void Tower::OnRender(SDL_Renderer* _renderer)
{
	//防御塔贴图的左上角顶点坐标
	static SDL_Point _point;
	_point.x = (int)(position.x - size.x / 2);
	_point.y = (int)(position.y - size.y / 2);
	//更新当前动画
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
	//开火的朝向
	Vector2 _direction = (_target->GetPosition() - this->position).Normalized();

	#pragma region UpdateTowerAnimation
	//根据开火朝向更新防御塔朝向
	UpdateFacingDir(_direction);
	//然后按照朝向更新当前动画为对应方向的开火动画
	UpdateFireAnim();
	#pragma endregion

	#pragma region InstantiateBullet
	//根据开火朝向实例化子弹，传入的速度向量记得乘上瓦片大小（这是由本工程中速率的定义所决定的）
	BulletManager::Instance()->FireBullet(bulletType, this->position, _direction * bulletSpeed * TILE_SIZE, bulletDamage);
	#pragma endregion
}

void Tower::UpdateFacingDir(const Vector2& _direction)
{
	//先判断是水平还是竖直
	bool _isHorizontal = abs(_direction.x) >= abs(_direction.y);

	//再选取具体的正负半轴方向
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

	//在攻击范围内的敌人中，应当优先攻击最靠近家门口的那个
	double _maxProcess = -1;
	for (Enemy* _enemy : _enemyList)
	{
		//确保敌人在攻击半径范围内
		if ((_enemy->GetPosition() - this->position).Length() <= fireRadius)
		{
			//不断遍历更新最大完成度，最终得到路径完成度最大的怪物，其距离家门口最近
			double _newProcess = _enemy->GetRouteProcess();
			if (_newProcess > _maxProcess)
			{
				_target = _enemy;
				_maxProcess = _newProcess;
			}
		}
	}

	//返回目标敌人指针
	return _target;
}
