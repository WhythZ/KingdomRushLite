#ifndef _BULLET_HPP_
#define _BULLET_HPP_

#include "../Math/Vector2.hpp"
#include "../Animation/Animation.hpp"
#include "../Manager/Concrete/ConfigManager.hpp"
#include "../Enemy/Enemy.hpp"

class Bullet
{
protected:
	#pragma region Transform
	Vector2 size;                           //子弹的尺寸
	Vector2 position;                       //子弹的位置
	#pragma endregion

	#pragma region Properties
	Vector2 velocity;                       //子弹运动的速度
	float damage = 0;                       //子弹能对敌人造成的伤害
	float damageRange = -1;                 //子弹造成伤害的AOE范围，
	#pragma endregion

	#pragma region Animation
	Animation animFlying;                   //子弹飞行时的动画
	bool canRotate;                         //子弹是否可旋转对动画有影响
	#pragma endregion

private:
	bool isValid = true;                    //决定子弹是否应当继续存在
	bool canCollide = true;                 //子弹是否可以被碰撞（伤害检定仅发生在碰撞那一帧，后续播放碰撞后动画时的帧不应继续检定）
	double rotationAngle = 0;               //描述子弹瞄准方向的欧拉角度，用于动画的处理

public:
	Bullet() = default;
	~Bullet() = default;

	void SetPosition(const Vector2&);       //设定子弹的发射位置
	void SetVelocity(const Vector2&);       //设定子弹发射的速度向量
	void SetDamage(double);                 //设定子弹命中敌人的伤害

	virtual void OnUpdate(double);
	virtual void OnRender(SDL_Renderer*);
	virtual void OnCollide(Enemy*);         //子弹碰撞检测的函数

	void DisableCollide();                  //取消子弹的可碰撞，以造成瞬时伤害而非持续伤害
	void Invalidate();                      //使得子弹消失

	const Vector2& GetSize() const;         //获取子弹尺寸，用于命中碰撞检测
	const Vector2& GetPosition() const;     //获取子弹位置
	double GetDamage() const;               //获取子弹伤害
	double GetDamageRange() const;          //获取子弹伤害范围，用于伤害碰撞检测
	bool CanCollide() const;                //查询子弹的可碰撞与否
	bool IsValid() const;                   //查询子弹的失效与否
};

void Bullet::SetPosition(const Vector2& _position)
{
	position = _position;
}

void Bullet::SetVelocity(const Vector2& _velocity)
{
	velocity = _velocity;

	//如果子弹可以旋转，则需考虑速度向量的方向，否则只考虑其大小标量
	if (canRotate)
	{
		//反三角函数获取弧度
		double _radian = std::atan2(velocity.y, velocity.x);
		//转换为欧拉角度，此处暂时使用硬编码表示\pi
		rotationAngle = _radian * 180 / 3.1415926;
	}
}

void Bullet::SetDamage(double _damage)
{
	damage = _damage;
}

void Bullet::OnUpdate(double _delta)
{
	//更新子弹飞行时的动画
	animFlying.OnUpdate(_delta);

	//按照速度向量更新子弹位置，并乘上单位时间使得实际移动速率与帧率无关
	position += velocity * _delta;

	//边缘检测，若子弹飞出地图（利用地图Rect进行检定）则及时清除
	static const SDL_Rect _mapRect = ConfigManager::Instance()->mapRect;
	if (position.x <= _mapRect.x + size.x / 2
		|| position.x >= _mapRect.x + _mapRect.w - size.x / 2
		|| position.y >= _mapRect.y + size.y / 2
		|| position.y <= _mapRect.y + _mapRect.h - size.y / 2)
	{
		Invalidate();
	}
}

void Bullet::OnRender(SDL_Renderer* _renderer)
{
	//子弹贴图的左上角顶点坐标（所有Bullet对象调用该函数时都会复用该静态变量，避免重复创建和销毁的开销）
	static SDL_Point _point;
	_point.x = (int)(position.x - size.x / 2);
	_point.y = (int)(position.y - size.y / 2);

	//调用当前动画的渲染，注意传入了旋转角度
	animFlying.OnRender(_renderer, _point, rotationAngle);
}

void Bullet::OnCollide(Enemy* _enemy)
{
	//与某个敌人碰撞后，子弹消失且不再重复发生碰撞
	Invalidate();
	DisableCollide();

	//对被碰撞到的敌人进行伤害检定
}

void Bullet::DisableCollide()
{
	canCollide = false;
}

void Bullet::Invalidate()
{
	isValid = false;
	//失效的同时也要取消碰撞，因为子弹失效时碰撞可能未失效（一定时间内未击中敌人）
	DisableCollide();
}

const Vector2& Bullet::GetSize() const
{
	return size;
}

const Vector2& Bullet::GetPosition() const
{
	return position;
}

double Bullet::GetDamage() const
{
	return damage;
}

double Bullet::GetDamageRange() const
{
	return damageRange;
}

bool Bullet::CanCollide() const
{
	return canCollide;
}

bool Bullet::IsValid() const
{
	return isValid;
}

#endif
