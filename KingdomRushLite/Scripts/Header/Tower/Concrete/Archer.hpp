#ifndef _ARCHER_HPP_
#define _ARCHER_HPP_

#include "../Tower.hpp"

class Archer : public Tower
{
public:
	Archer();
	~Archer() = default;
};

Archer::Archer()
{
	//指定防御塔类型
	type = TowerType::Archer;
}

#endif
