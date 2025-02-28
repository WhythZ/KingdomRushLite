#ifndef _PLAYER_SKILL_STATE_H_
#define _PLAYER_SKILL_STATE_H_

#include "../PlayerState.h"

class PlayerSkillState :public PlayerState
{
public:
	PlayerSkillState(bool*);
	~PlayerSkillState() = default;

	void OnBegin() override;
	void OnInput(const SDL_Event&) override;
	void OnUpdate(double) override;
	void OnRender(SDL_Renderer*) override;
	void OnEnd() override;
};

#endif
