#ifndef _AXEMAN_H_
#define _AXEMAN_H_

#include "../Tower.h"

#define AXEMAN_BULLET_SPEED 5

class Axeman : public Tower
{
public:
	Axeman();
	~Axeman() = default;

protected:
	void OnFireBullet() override;        //��д�����й����ӵ�������߼�
};

#endif
