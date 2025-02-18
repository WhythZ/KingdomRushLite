#ifndef _GUNNER_HPP_
#define _GUNNER_HPP_

#include "../Tower.hpp"

#define GUNNER_BULLET_SPEED 6

class Gunner : public Tower
{
public:
	Gunner();
	~Gunner() = default;

protected:
	void OnFireBullet() override;        //重写基类中关于子弹发射的逻辑
};

#endif
