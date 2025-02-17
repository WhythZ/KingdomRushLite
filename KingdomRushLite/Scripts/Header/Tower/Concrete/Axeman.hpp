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
	//指定防御塔类型
	type = TowerType::Axeman;
}

#endif
