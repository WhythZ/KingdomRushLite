#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

#include "State.h"
#include "../Vector2.h"

class StateMachine
{
private:
	State* currentState = nullptr;           //״̬����ǰ���ڸ��µ�״̬
	//State* formerState = nullptr;

public:
	StateMachine() = default;
	~StateMachine() = default;

	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	void ChangeState(State*);

	State* GetCurrentState() const;
};

#endif

