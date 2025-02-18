#ifndef _ARCHER_H_
#define _ARCHER_H_

#include "../Tower.h"

#define ARCHER_BULLET_SPEED 6

class Archer : public Tower
{
public:
	Archer();
	~Archer() = default;

protected:
	void OnFireBullet(Enemy*) override;        //重写基类中关于子弹发射的逻辑
};

#endif
