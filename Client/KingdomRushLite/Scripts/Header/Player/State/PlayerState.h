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
	PlayerState(bool*);
	~PlayerState() = default;

	virtual void OnBegin() override;
	virtual void OnUpdate(double) override;
	virtual void OnRender(SDL_Renderer*) override;
	virtual void OnEnd() override;
};

#endif
