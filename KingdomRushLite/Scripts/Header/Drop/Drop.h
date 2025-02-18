#ifndef _DROP_H_
#define _DROP_H_

#include "../Math/Vector2.h"
#include "../Math/Timer.h"
#include "DropType.h"

class Drop
{
protected:
	DropType type = DropType::None;

	#pragma region Transform
	Vector2 size;               //掉落物的尺寸
	Vector2 position;           //掉落物出现的位置
	#pragma endregion

	#pragma region Properties
	Vector2 velocity;           //掉落物是从初始位置向上弹出来的，故需控制其速度向量
	double gravity = 10;        //掉落物的重力，控制其模拟的速度改变的强度
	#pragma endregion

private:
	bool isValid = true;        //掉落物是否应当继续存在于场景中
	bool isPoping = false;      //掉落物是否处于弹出的状态（此时速度不应为0）

	double popTime = 0.75;      //掉落物上升的时间
	double existTime = 10;      //掉落物存在的时间
	Timer popTimer;             //控制掉落物停止上升，模拟其向上弹出后下坠的重力效果
	Timer disappearTimer;       //控制掉落物经过一定时间后自动消失

public:
	Drop();
	~Drop() = default;

	void OnUpdate(double);
	void OnRender(SDL_Renderer);
	void OnCollide();
};

#endif
