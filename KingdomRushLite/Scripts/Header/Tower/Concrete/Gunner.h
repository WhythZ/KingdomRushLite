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
	void OnFireBullet(Enemy*) override;        //��д�����й����ӵ�������߼�
};

#endif
