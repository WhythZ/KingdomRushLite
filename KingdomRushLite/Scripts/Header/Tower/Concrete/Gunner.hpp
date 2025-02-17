#ifndef _GUNNER_HPP_
#define _GUNNER_HPP_

#include "../Tower.hpp"

class Gunner : public Tower
{
public:
	Gunner();
	~Gunner() = default;
};

Gunner::Gunner()
{
	//指定防御塔类型
	type = TowerType::Gunner;
}

#endif
