#ifndef _PLAYER_SKILL_STATE_H_
#define _PLAYER_SKILL_STATE_H_

#include "../PlayerState.h"

class PlayerSkillState :public PlayerState
{
public:
	PlayerSkillState();
	~PlayerSkillState() = default;

	void OnBegin() override;
	void OnUpdate(double) override;
	void OnRender(SDL_Renderer*) override;
	void OnEnd() override;
};

#endif
