#include "../../Header/Bullet/Bullet.hpp"
#include "../../Header/Manager/Concrete/ProcessManager.hpp"

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
	static const SDL_Rect _mapRect = ProcessManager::Instance()->mapRect;
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
