#ifndef _AXE_H_
#define _AXE_H_

#include "../Bullet.h"



class Axe : public Bullet
{
private:
	double slowDownPercentage = 0.3;      //减缓怪物的多少移速
	double slowDownDuration = 1;          //移速减缓的持续时间

public:
	Axe();
	~Axe() = default;

	void OnCollide(Enemy*) override;
};

#endif
