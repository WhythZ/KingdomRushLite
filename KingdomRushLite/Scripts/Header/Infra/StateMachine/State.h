#ifndef _STATE_H_
#define _STATE_H_

#include "../Vector2.h"
#include "../Animation.h"

class State
{
public:
	State() = default;
	~State() = default;

	virtual void OnBegin() = 0;                //��ʼ��ǰ״̬ʱ����
	virtual void OnUpdate(double) = 0;         //��ǰ״̬�����и���
	virtual void OnRender(SDL_Renderer*) = 0;  //��ǰ״̬�����и���
	virtual void OnEnd() = 0;                  //������ǰ״̬ʱ����
};

#endif
