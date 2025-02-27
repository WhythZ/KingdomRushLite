#ifndef _COIN_H_
#define _COIN_H_

#include "../Drop.h"

class Coin :public Drop
{
private:
	double coinReward = 20;        //奖励的金币数量

public:
	Coin();
	~Coin() = default;

	void OnCollide() override;
};

#endif
