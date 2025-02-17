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
	TowerType type = TowerType::None;          //防御塔的种类

	#pragma region Transform
	Vector2 size;                              //用于渲染与碰撞的尺寸
	#pragma endregion

	#pragma region Animation
	Animation animIdleUp;              //待机朝上的动画
	Animation animIdleDown;            //待机朝下的动画
	Animation animIdleLeft;            //待机朝左的动画
	Animation animIdleRight;           //待机朝右的动画

	Animation animFireUp;              //攻击朝上的动画
	Animation animFireDown;            //攻击朝下的动画
	Animation animFireLeft;            //攻击朝左的动画
	Animation animFireRight;           //攻击朝右的动画
	#pragma endregion

	#pragma region Properties
	float fireCooldown = 0;                    //每次发射的间隔
	float fireRadius = 0;                      //攻击范围半径
	BulletType bulletType = BulletType::None;  //子弹的类型
	float bulletDamage = 0;                    //子弹的伤害
	float bulletSpeed = 0;                     //子弹的速度
	#pragma endregion

private:
	Vector2 position;                          //防御塔所在位置
	
	Animation* animCurrent = &animIdleDown;    //防御塔默认动画
	FacingDir facingDir = FacingDir::Down;     //防御塔默认动画的朝向

	bool canFire = true;                       //决定是否可以发射子弹
	Timer fireTimer;                           //控制子弹发射间隔的定时器

public:
	Tower();
	~Tower() = default;
	void SetPosition(const Vector2&);          //设置防御塔的位置

	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	const Vector2& GetSize() const;            //获取防御塔的尺寸
	const Vector2& GetPosition() const;        //获取防御塔的位置
		
protected:
	void virtual UpdateBulletData() = 0;       //更新子弹属性的纯虚函数
	void virtual OnFireBullet();               //子弹发射时的逻辑

private:
	void UpdateFacingDir(const Vector2&);      //更新防御塔朝向
	void UpdateIdleAnim();                     //依照朝向更新当前动画为对应闲置动画
	void UpdateFireAnim();                     //依照朝向更新当前动画为对应开火动画
	Enemy* FindTargetEnemy();                  //子弹索敌的逻辑
};

Tower::Tower()
{
	#pragma region Animation
	//初始化动画的属性（纹理在子类中设置）
	animIdleUp.SetLoop(true);
	animIdleDown.SetLoop(true);
	animIdleLeft.SetLoop(true);
	animIdleRight.SetLoop(true);
	animFireUp.SetLoop(false);
	animFireDown.SetLoop(false);
	animFireLeft.SetLoop(false);
	animFireRight.SetLoop(false);

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

	//更新子弹的属性数据，包括位置与瞬时速度向量
	UpdateBulletData();

	//如果处于可开火的状态，则准备用于开火的数据
	if (canFire)
	{
		//调用子弹发射逻辑
		OnFireBullet();

		//重置可开火状态
		canFire = false;
		//重置计时器，等待经过冷却后canFire被回调函数重新赋值为true
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

void Tower::OnFireBullet()
{
	//如果暂时未找到可攻击目标，则无需进行后续更新浪费资源
	Enemy* _target = FindTargetEnemy();
	if (!_target)
		return;

	//开火的朝向
	Vector2 _direction = (_target->GetPosition() - this->position).Normalized();

	#pragma region UpdateAnimation
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
	double _maxProcess = 0;
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

#endif
