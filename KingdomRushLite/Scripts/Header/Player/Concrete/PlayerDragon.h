#ifndef _PLAYER_DRAGON_H_
#define _PLAYER_DRAGON_H_

#include "../Player.h"

class PlayerDragon :public Player
{
public:
	PlayerDragon();
	~PlayerDragon() = default;

protected:
	void ReleaseSkill00() override;
	void ReleaseSkill01() override;
};

#endif