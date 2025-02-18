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
	void OnFireBullet() override;        //��д�����й����ӵ�������߼�
};

#endif
