#ifndef _AXEMAN_H_
#define _AXEMAN_H_

#include "../Tower.h"

#define TOWER_AXEMAN_BULLET_SPEED 5

class Axeman : public Tower
{
public:
	Axeman();
	~Axeman() = default;

protected:
	void OnFireBullet(Enemy*) override;        //重写基类中关于子弹发射的逻辑
};

#endif
