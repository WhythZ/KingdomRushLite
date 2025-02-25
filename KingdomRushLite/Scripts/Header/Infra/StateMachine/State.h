#ifndef _STATE_H_
#define _STATE_H_

#include "StateMachine.h"
#include "../Animation.h"

class State
{
protected:
	StateMachine* stateMachine;			     //��¼���Ƹ�״̬��״̬��
	Animation* animation;                    //��״̬���µĶ���

public:
	State(StateMachine*, Animation*);
	~State() = default;

	virtual void OnBegin() = 0;              //��ʼ��ǰ״̬ʱ����
	virtual void OnUpdate(double);           //״̬�����и���
	virtual void OnRender(SDL_Renderer*);    //״̬�����и���
	virtual void OnEnd() = 0;                //������ǰ״̬ʱ����
};

#endif
