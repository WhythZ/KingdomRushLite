#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

#include "State.h"
#include "../Vector2.h"

class StateMachine
{
protected:
	State* currentState = nullptr;           //状态机当前正在更新的状态
	//State* formerState = nullptr;

public:
	StateMachine() = default;
	~StateMachine() = default;

	void OnInput(const SDL_Event&);
	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	void ChangeState(State*);

	State* GetCurrentState() const;
};

#endif

