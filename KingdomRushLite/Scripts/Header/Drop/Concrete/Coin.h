#ifndef _COIN_H_
#define _COIN_H_

#include "../Drop.h"

class Coin :public Drop
{
private:
	double coinReward = 10;        //�����Ľ������

public:
	Coin();
	~Coin() = default;

	void OnCollide() override;
};

#endif
