#ifndef _GUNNER_H_
#define _GUNNER_H_

#include "../Tower.h"

#define GUNNER_BULLET_SPEED 6

class Gunner : public Tower
{
public:
	Gunner();
	~Gunner() = default;

protected:
	void OnFireBullet(Enemy*) override;        //重写基类中关于子弹发射的逻辑
};

#endif
