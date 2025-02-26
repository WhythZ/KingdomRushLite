#ifndef _PLAYER_STATE_H_
#define _PLAYER_STATE_H_

#include "../../Infra/StateMachine/State.h"
#include "../../Player/Player.h"

class PlayerState :public State
{
public:
	Animation animUp;
	Animation animDown;
	Animation animLeft;
	Animation animRight;

protected:
	Player* player;

	Animation* animCurrent;

	#pragma region Configs
	double speed;
	#pragma endregion

public:
	PlayerState(Player*);
	~PlayerState() = default;

	virtual void OnBegin() = 0;
	void OnUpdate(double) override;
	void OnRender(SDL_Renderer*) override;
	virtual void OnEnd() override = 0;
};

#endif
