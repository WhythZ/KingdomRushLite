#ifndef _PLAYER_IDLE_STATE_H_
#define _PLAYER_IDLE_STATE_H_

#include "../PlayerState.h"

class PlayerIdleState :public PlayerState
{
public:
	PlayerIdleState(bool*);
	~PlayerIdleState() = default;

	void OnBegin() override;
	void OnInput(const SDL_Event&) override;
	void OnUpdate(double) override;
	void OnRender(SDL_Renderer*) override;
	void OnEnd() override;
};

#endif
