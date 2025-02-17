#ifndef _TOWER_MANAGER_HPP_
#define _TOWER_MANAGER_HPP_

#include "../Manager.hpp"
#include "ProcessManager.hpp"
#include "../../Tower/Tower.hpp"
#include "../../Tower/TowerType.hpp"
#include "../../Tower/Concrete/Archer.hpp"
#include "../../Tower/Concrete/Axeman.hpp"
#include "../../Tower/Concrete/Gunner.hpp"

class TowerManager : public Manager<TowerManager>
{
	friend class Manager<TowerManager>;

public:
	typedef std::vector<Tower*> TowerList;

private:
	TowerList towerList;

public:
	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	void PlaceTower(TowerType, const SDL_Point&);
	void UpgradeTower(TowerType);

	double GetPlaceCostOf(TowerType) const;
	double GetUpgradeCostOf(TowerType) const;
	double GetFireCooldownOf(TowerType) const;
	double GetFireRadiusOf(TowerType) const;
	double GetBulletDamageOf(TowerType) const;

private:
	TowerManager() = default;
	~TowerManager() = default;
};

void TowerManager::OnUpdate(double _delta)
{

}

void TowerManager::OnRender(SDL_Renderer* _renderer)
{

}

void TowerManager::PlaceTower(TowerType _type, const SDL_Point& _point)
{

}

void TowerManager::UpgradeTower(TowerType _type)
{

}

double TowerManager::GetPlaceCostOf(TowerType _type) const
{
	static ConfigManager* _config = ConfigManager::Instance();
	static ProcessManager* _process = ProcessManager::Instance();

	switch (_type)
	{
	case TowerType::None:
		return -1;
		break;
	case TowerType::Archer:
		return _config->archerPrefab.buildCost[_process->levelArcher];
		break;
	case TowerType::Axeman:
		return _config->axemanPrefab.buildCost[_process->levelAxeman];
		break;
	case TowerType::Gunner:
		return _config->gunnerPrefab.buildCost[_process->levelGunner];
		break;
	}
}

double TowerManager::GetUpgradeCostOf(TowerType _type) const
{
	static ConfigManager* _config = ConfigManager::Instance();
	static ProcessManager* _process = ProcessManager::Instance();

	switch (_type)
	{
	case TowerType::None:
		return -1;
		break;
	case TowerType::Archer:
		return _config->archerPrefab.upgradeCost[_process->levelArcher];
		break;
	case TowerType::Axeman:
		return _config->axemanPrefab.upgradeCost[_process->levelAxeman];
		break;
	case TowerType::Gunner:
		return _config->gunnerPrefab.upgradeCost[_process->levelGunner];
		break;
	}
}

double TowerManager::GetFireCooldownOf(TowerType _type) const
{
	static ConfigManager* _config = ConfigManager::Instance();
	static ProcessManager* _process = ProcessManager::Instance();

	switch (_type)
	{
	case TowerType::None:
		return -1;
		break;
	case TowerType::Archer:
		return _config->archerPrefab.cooldown[_process->levelArcher];
		break;
	case TowerType::Axeman:
		return _config->axemanPrefab.cooldown[_process->levelAxeman];
		break;
	case TowerType::Gunner:
		return _config->gunnerPrefab.cooldown[_process->levelGunner];
		break;
	}
}

double TowerManager::GetFireRadiusOf(TowerType _type) const
{
	static ConfigManager* _config = ConfigManager::Instance();
	static ProcessManager* _process = ProcessManager::Instance();

	//此处半径的大小的定义是以瓦片格为单位的，所以要乘上瓦片尺寸
	switch (_type)
	{
	case TowerType::None:
		return -1;
		break;
	case TowerType::Archer:
		return _config->archerPrefab.viewRange[_process->levelArcher] * TILE_SIZE;
		break;
	case TowerType::Axeman:
		return _config->axemanPrefab.viewRange[_process->levelAxeman] * TILE_SIZE;
		break;
	case TowerType::Gunner:
		return _config->gunnerPrefab.viewRange[_process->levelGunner] * TILE_SIZE;
		break;
	}
}

double TowerManager::GetBulletDamageOf(TowerType _type) const
{
	static ConfigManager* _config = ConfigManager::Instance();
	static ProcessManager* _process = ProcessManager::Instance();

	switch (_type)
	{
	case TowerType::None:
		return -1;
		break;
	case TowerType::Archer:
		return _config->archerPrefab.damage[_process->levelArcher];
		break;
	case TowerType::Axeman:
		return _config->axemanPrefab.damage[_process->levelAxeman];
		break;
	case TowerType::Gunner:
		return _config->gunnerPrefab.damage[_process->levelGunner];
		break;
	}
}

#endif
