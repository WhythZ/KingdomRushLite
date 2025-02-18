#ifndef _ARCHER_HPP_
#define _ARCHER_HPP_

#include "../Tower.hpp"

#define ARCHER_BULLET_SPEED 6

class Archer : public Tower
{
public:
	Archer();
	~Archer() = default;

protected:
	void OnFireBullet() override;        //重写基类中关于子弹发射的逻辑
};

#endif
