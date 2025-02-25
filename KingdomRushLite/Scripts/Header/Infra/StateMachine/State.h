#ifndef _STATE_H_
#define _STATE_H_

#include "StateMachine.h"
#include "../Animation.h"

class State
{
protected:
	StateMachine* stateMachine;			     //记录控制该状态的状态机
	Animation* animation;                    //该状态更新的动画

public:
	State(StateMachine*, Animation*);
	~State() = default;

	virtual void OnBegin() = 0;              //开始当前状态时调用
	virtual void OnUpdate(double);           //状态进行中更新
	virtual void OnRender(SDL_Renderer*);    //状态进行中更新
	virtual void OnEnd() = 0;                //结束当前状态时调用
};

#endif
