#ifndef _AXEMAN_HPP_
#define _AXEMAN_HPP_

#include "../Tower.hpp"

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
