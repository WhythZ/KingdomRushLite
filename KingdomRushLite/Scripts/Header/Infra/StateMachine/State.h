#ifndef _STATE_H_
#define _STATE_H_

#include "../Vector2.h"
#include "../Animation.h"

class State
{
protected:
	bool* flag = nullptr;                        //指向外部的某个代表正处于当前状态的布尔值
	Timer stateTimer;                            //记录进入状态的时间

public:
	State(bool*);
	~State() = default;

	virtual void OnBegin();                      //开始当前状态时调用
	virtual void OnInput(const SDL_Event&) = 0;  //当前状态进行中更新
	virtual void OnUpdate(double);               //当前状态进行中更新
	virtual void OnRender(SDL_Renderer*) = 0;    //当前状态进行中更新
	virtual void OnEnd();                        //结束当前状态时调用
};

#endif
