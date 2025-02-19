#ifndef _DROP_H_
#define _DROP_H_

#include "DropType.h"
#include "../Math/Vector2.h"
#include "../Math/Timer.h"
#include "../Animation/Animation.h"

class Drop
{
protected:
	DropType type = DropType::None;

	#pragma region Transform
	Vector2 size;                      //掉落物的尺寸
	Vector2 position;                  //掉落物出现的位置
	#pragma endregion

	#pragma region Properties
	Vector2 velocity;                  //掉落物是从初始位置向上弹出来的，故需控制其速度向量
	double gravity = 10;               //掉落物的重力加速度
	double popDuration = 1;            //掉落物上升的时间
	double existDuration = 10;         //掉落物存在的时间
	double initSpeedX = 2;             //掉落物生成时的初始水平速率
	double initSpeedY = 4;             //掉落物生成时的初始纵向速率
	double floatingFrequency = 4;      //与三角函数自变量相乘，控制掉落物在地面上的波动频率
	double floatingAmplitude = 16;     //与三角函数值相乘，控制掉落物在地面上的波动振幅
	#pragma endregion

	#pragma region Animation
	Animation animIdle;                //掉落物闲置状态动画
	#pragma endregion

private:
	bool isValid = true;               //掉落物是否应当继续存在于场景中
	bool isPoping = false;             //掉落物是否处于弹出的状态（此时速度不应为0）

	Timer popTimer;                    //控制掉落物停止上升，模拟其向上弹出后下坠的重力效果
	Timer disappearTimer;              //控制掉落物经过一定时间后自动消失

public:
	Drop();
	~Drop() = default;
	void SetPosition(const Vector2&);

	void OnUpdate(double);
	void OnRender(SDL_Renderer*);
	void virtual OnCollide();

	void Invalidate();

	bool IsValid() const;
	const Vector2& GetPosition() const;
	const Vector2& GetSize() const;
};

#endif
