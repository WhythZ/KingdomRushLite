#ifndef _AXEMAN_HPP_
#define _AXEMAN_HPP_

#include "../Tower.hpp"

class Axeman : public Tower
{
public:
	Axeman();
	~Axeman() = default;
};

Axeman::Axeman()
{
	//ָ������������
	type = TowerType::Axeman;
}

#endif
