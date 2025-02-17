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

	void BuildTower(TowerType, const SDL_Point&);
	void UpgradeTower(TowerType);

	double GetBuildCostOf(TowerType) const;
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
	for (Tower* _tower : towerList)
		_tower->OnUpdate(_delta);
}

void TowerManager::OnRender(SDL_Renderer* _renderer)
{
	for (Tower* _tower : towerList)
		_tower->OnRender(_renderer);
}

void TowerManager::BuildTower(TowerType _type, const SDL_Point& _point)
{
	Tower* _tower = nullptr;
	switch (_type)
	{
	case TowerType::Archer:
		_tower = new Archer();
		break;
	case TowerType::Axeman:
		_tower = new Axeman();
		break;
	case TowerType::Gunner:
		_tower = new Gunner();
		break;
	default:
	}
	if (_tower == nullptr)
		return;

	//���ô������ɢ�����꣬��ȡ��Ƭ��ͼ�ж�Ӧ����Ƭλ�ã���������ʵ�ʶ�λ�ڸ�λ����
	static Vector2 _position;
	static const SDL_Rect& _mapRect = ConfigManager::Instance()->mapRect;
	_position.x = _mapRect.x + _point.x * TILE_SIZE + TILE_SIZE / 2;
	_position.x = _mapRect.y + _point.y * TILE_SIZE + TILE_SIZE / 2;
	_tower->SetPosition(_position);

	//��ʵ�����ķ�������������б�
	towerList.push_back(_tower);

	//���õ�ͼ��������ض���Ƭ���Ѿ����ڷ�����
	ConfigManager::Instance()->map.MarkTowerExistenceAt(_point);

	//���ŷ��÷���������Ч
	AudioManager::Instance()->PlaySFX(SoundResID::Tower_Place);
}

void TowerManager::UpgradeTower(TowerType _type)
{
	static ProcessManager* _process = ProcessManager::Instance();

	//��߼�Ϊʮ��
	switch (_type)
	{
	case TowerType::Archer:
		_process->levelArcher = (_process->levelArcher >= 9) ? 9 : (_process->levelArcher + 1);
		break;
	case TowerType::Axeman:
		_process->levelAxeman = (_process->levelAxeman >= 9) ? 9 : (_process->levelAxeman + 1);
		break;
	case TowerType::Gunner:
		_process->levelGunner = (_process->levelGunner >= 9) ? 9 : (_process->levelGunner + 1);
		break;
	default:
	}

	//������������������Ч
	AudioManager::Instance()->PlaySFX(SoundResID::Tower_Upgrade);
}

double TowerManager::GetBuildCostOf(TowerType _type) const
{
	static ConfigManager* _config = ConfigManager::Instance();
	static ProcessManager* _process = ProcessManager::Instance();

	switch (_type)
	{
	case TowerType::Archer:
		return _config->archerPrefab.buildCost[_process->levelArcher];
		break;
	case TowerType::Axeman:
		return _config->axemanPrefab.buildCost[_process->levelAxeman];
		break;
	case TowerType::Gunner:
		return _config->gunnerPrefab.buildCost[_process->levelGunner];
		break;
	default:
		return -1;
	}
}

double TowerManager::GetUpgradeCostOf(TowerType _type) const
{
	static ConfigManager* _config = ConfigManager::Instance();
	static ProcessManager* _process = ProcessManager::Instance();

	//���������ȼ��������ֵ����ʮ��������Ϊ9����Ӧ������һ��������ֵ��-1��ʾ�޷����������������Ƿ���������ֵ���޷��ж��Ƿ񵽴����ֵ��
	switch (_type)
	{
	case TowerType::Archer:
		return (_process->levelArcher == 9) ? -1 : _config->archerPrefab.upgradeCost[_process->levelArcher];
		break;
	case TowerType::Axeman:
		return (_process->levelAxeman == 9) ? -1 : _config->axemanPrefab.upgradeCost[_process->levelAxeman];
		break;
	case TowerType::Gunner:
		return (_process->levelGunner == 9) ? -1 : _config->gunnerPrefab.upgradeCost[_process->levelGunner];
		break;
	default:
		return -1;
	}
}

double TowerManager::GetFireCooldownOf(TowerType _type) const
{
	static ConfigManager* _config = ConfigManager::Instance();
	static ProcessManager* _process = ProcessManager::Instance();

	switch (_type)
	{
	case TowerType::Archer:
		return _config->archerPrefab.cooldown[_process->levelArcher];
		break;
	case TowerType::Axeman:
		return _config->axemanPrefab.cooldown[_process->levelAxeman];
		break;
	case TowerType::Gunner:
		return _config->gunnerPrefab.cooldown[_process->levelGunner];
		break;
	default:
		return -1;
	}
}

double TowerManager::GetFireRadiusOf(TowerType _type) const
{
	static ConfigManager* _config = ConfigManager::Instance();
	static ProcessManager* _process = ProcessManager::Instance();

	//�˴��뾶�Ĵ�С�Ķ���������Ƭ��Ϊ��λ�ģ�����Ҫ������Ƭ�ߴ�
	switch (_type)
	{
	case TowerType::Archer:
		return _config->archerPrefab.viewRange[_process->levelArcher] * TILE_SIZE;
		break;
	case TowerType::Axeman:
		return _config->axemanPrefab.viewRange[_process->levelAxeman] * TILE_SIZE;
		break;
	case TowerType::Gunner:
		return _config->gunnerPrefab.viewRange[_process->levelGunner] * TILE_SIZE;
		break;
	default:
		return -1;
	}
}

double TowerManager::GetBulletDamageOf(TowerType _type) const
{
	static ConfigManager* _config = ConfigManager::Instance();
	static ProcessManager* _process = ProcessManager::Instance();

	switch (_type)
	{
	case TowerType::Archer:
		return _config->archerPrefab.damage[_process->levelArcher];
		break;
	case TowerType::Axeman:
		return _config->axemanPrefab.damage[_process->levelAxeman];
		break;
	case TowerType::Gunner:
		return _config->gunnerPrefab.damage[_process->levelGunner];
		break;
	default:
		return -1;
	}
}

#endif
