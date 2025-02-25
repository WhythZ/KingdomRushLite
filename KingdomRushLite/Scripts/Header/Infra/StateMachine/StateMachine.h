#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

#include "State.h"
#include "../Vector2.h"

class StateMachine
{
public:
	Vector2* position;                      //记录状态机主人的中心点位置
	Vector2* size;                          //记录状态机主人的尺寸，用于动画渲染

private:
	State* currentState = nullptr;          //状态机当前正在更新的状态
	//State* formerState = nullptr;

public:
	StateMachine(Vector2*, Vector2*);       //传入主人位置与尺寸实例化状态机
	~StateMachine() = default;

	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	void ChangeState(State*);

	State* GetCurrentState() const;
};

#endif

