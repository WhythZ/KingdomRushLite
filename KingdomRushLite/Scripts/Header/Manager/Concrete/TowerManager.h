#ifndef _TOWER_MANAGER_H_
#define _TOWER_MANAGER_H_

#include "../Manager.hpp"
#include "../../Tower/Tower.h"

class TowerManager :public Manager<TowerManager>
{
	friend class Manager<TowerManager>;

public:
	typedef std::vector<Tower*> TowerList;

private:
	TowerList towerList;

public:
	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	void BuildTower(TowerType, const SDL_Point&);
	void UpgradeTower(TowerType);

	double GetBuildCostOf(TowerType) const;
	double GetUpgradeCostOf(TowerType) const;
	double GetFireCooldownOf(TowerType) const;
	double GetFireRadiusOf(TowerType) const;
	double GetBulletDamageOf(TowerType) const;

private:
	TowerManager() = default;
	~TowerManager();
};

#endif
