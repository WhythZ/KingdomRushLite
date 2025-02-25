#ifndef _PLAYER_DRAGON_H_
#define _PLAYER_DRAGON_H_

#include "../Player.h"

class PlayerDragon :public Player
{
public:
	PlayerDragon();
	~PlayerDragon() = default;

	void OnInput(const SDL_Event&) override;
	void OnUpdate(double) override;
	void OnRender(SDL_Renderer*) override;

protected:
	void ReleaseSkill00() override;
	void ReleaseSkill01() override;
};

#endif