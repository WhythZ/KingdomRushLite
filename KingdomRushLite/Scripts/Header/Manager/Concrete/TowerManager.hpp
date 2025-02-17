#ifndef _TOWER_MANAGER_HPP_
#define _TOWER_MANAGER_HPP_

#include "../Manager.hpp"
#include "../../Tower/Tower.hpp"
#include "../../Tower/TowerType.hpp"
#include "../../Tower/Concrete/Archer.hpp"
#include "../../Tower/Concrete/Axeman.hpp"
#include "../../Tower/Concrete/Gunner.hpp"

class TowerManager : public Manager<TowerManager>
{
	friend class Manager<TowerManager>;

private:
	TowerManager() = default;
	~TowerManager() = default;
};

#endif
