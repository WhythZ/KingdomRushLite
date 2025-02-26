#ifndef _STATE_H_
#define _STATE_H_

#include "../Vector2.h"
#include "../Animation.h"

class State
{
public:
	State() = default;
	~State() = default;

	virtual void OnBegin() = 0;                //开始当前状态时调用
	virtual void OnUpdate(double) = 0;         //当前状态进行中更新
	virtual void OnRender(SDL_Renderer*) = 0;  //当前状态进行中更新
	virtual void OnEnd() = 0;                  //结束当前状态时调用
};

#endif
