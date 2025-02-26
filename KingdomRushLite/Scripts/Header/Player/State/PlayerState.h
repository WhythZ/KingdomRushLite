#ifndef _PLAYER_STATE_H_
#define _PLAYER_STATE_H_

#include "../../Infra/StateMachine/State.h"

class PlayerState :public State
{
public:
	Animation animUp;
	Animation animDown;
	Animation animLeft;
	Animation animRight;

protected:
	Animation* animCurrent;

public:
	PlayerState();
	~PlayerState() = default;

	virtual void OnBegin() = 0;
	virtual void OnUpdate(double) override;
	virtual void OnRender(SDL_Renderer*) override;
	virtual void OnEnd() = 0;
};

#endif
