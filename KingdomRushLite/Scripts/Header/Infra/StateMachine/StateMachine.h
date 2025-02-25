#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

#include "State.h"
#include "../Vector2.h"

class StateMachine
{
public:
	Vector2* position;                      //��¼״̬�����˵����ĵ�λ��
	Vector2* size;                          //��¼״̬�����˵ĳߴ磬���ڶ�����Ⱦ

private:
	State* currentState = nullptr;          //״̬����ǰ���ڸ��µ�״̬
	//State* formerState = nullptr;

public:
	StateMachine(Vector2*, Vector2*);       //��������λ����ߴ�ʵ����״̬��
	~StateMachine() = default;

	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	void ChangeState(State*);

	State* GetCurrentState() const;
};

#endif

