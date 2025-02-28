#ifndef _STATE_H_
#define _STATE_H_

#include "../Vector2.h"
#include "../Animation.h"

class State
{
protected:
	bool* flag = nullptr;                        //ָ���ⲿ��ĳ�����������ڵ�ǰ״̬�Ĳ���ֵ
	Timer stateTimer;                            //��¼����״̬��ʱ��

public:
	State(bool*);
	~State() = default;

	virtual void OnBegin();                      //��ʼ��ǰ״̬ʱ����
	virtual void OnInput(const SDL_Event&) = 0;  //��ǰ״̬�����и���
	virtual void OnUpdate(double);               //��ǰ״̬�����и���
	virtual void OnRender(SDL_Renderer*) = 0;    //��ǰ״̬�����и���
	virtual void OnEnd();                        //������ǰ״̬ʱ����
};

#endif
