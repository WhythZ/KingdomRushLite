#ifndef _PLAYER_MOVE_STATE_H_
#define _PLAYER_MOVE_STATE_H_

#include "../PlayerState.h"

class PlayerMoveState :public PlayerState
{
public:
	PlayerMoveState();
	~PlayerMoveState() = default;

	void OnBegin() override;
	void OnUpdate(double) override;
	void OnRender(SDL_Renderer*) override;
	void OnEnd() override;
};

#endif
