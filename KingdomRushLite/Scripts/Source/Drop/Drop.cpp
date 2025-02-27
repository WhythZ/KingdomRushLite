#include "../../Header/Drop/Drop.h"
#include "../../Header/Map/Tile.h"

Drop::Drop()
{
	#pragma region Timer
	popTimer.SetOneShot(true);
	popTimer.SetWaitTime(popDuration);
	popTimer.SetTimeOutTrigger(
		[&]()
		{
			isPoping = false;
		}
	);

	disappearTimer.SetOneShot(true);
	disappearTimer.SetWaitTime(existDuration);
	disappearTimer.SetTimeOutTrigger(
		[&]()
		{
			Invalidate();
		}
	);
	#pragma endregion

	#pragma region Velocity
	//当掉落物生成时，随机（rand()%2随机返回0或1作布尔值）赋予水平向左或向右的水平速度，根据本项目速度定义乘上瓦片尺寸
	velocity.x = (rand() % 2 ? 1 : -1) * initSpeedX * TILE_SIZE;
	//再赋予初始的向上（所以有负号）的纵向速度
	velocity.y = -initSpeedY * TILE_SIZE;
	#pragma endregion
}

void Drop::SetPosition(const Vector2& _position)
{
	position = _position;
}

void Drop::OnUpdate(double _delta)
{
	popTimer.OnUpdate(_delta);
	disappearTimer.OnUpdate(_delta);

	animIdle.OnUpdate(_delta);

	//如果该掉落物是刚掉落出来处于向上弹出的状态，则对其向下（注意是加号）施加重力加速度，模拟自由落体运动
	if (isPoping)
		velocity.y += gravity * _delta;
	//否则物体处于落地状态
	else
	{
		//阻止水平方向运动
		velocity.x = 0;

		//以三角函数模拟处于地面上的上下波动状态，暗示掉落物可以被捡起
		//SDL_GetTicks64()记录从SDL调用以来的时间，将其变换后用作自变量，最后将取值[-1,1]的三角函数值乘上振幅
		velocity.y = sin(SDL_GetTicks64() / 1000.0 * floatingFrequency) * floatingAmplitude;
	}

	//依据速度向量更新掉落物的位置
	position += velocity * _delta;
}

void Drop::OnRender(SDL_Renderer* _renderer)
{
	static SDL_Point _point;
	_point.x = (int)(position.x - size.x / 2);
	_point.y = (int)(position.y - size.y / 2);

	animIdle.OnRender(_renderer, _point);
}

void Drop::OnCollide()
{
	//清除自身
	Invalidate();
}

void Drop::Invalidate()
{
	isValid = false;
}

bool Drop::IsValid() const
{
	return isValid;
}

const Vector2& Drop::GetPosition() const
{
	return position;
}

const Vector2& Drop::GetSize() const
{
	return size;
}