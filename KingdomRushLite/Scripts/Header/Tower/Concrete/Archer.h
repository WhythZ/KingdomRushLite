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
	void OnFireBullet(Enemy*) override;        //��д�����й����ӵ�������߼�
};

#endif
