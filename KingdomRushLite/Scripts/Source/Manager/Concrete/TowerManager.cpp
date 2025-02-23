#include "../../../Header/Manager/Concrete/TowerManager.h"
#include "../../../Header/Manager/Concrete/ProcessManager.h"
#include "../../../Header/Manager/Concrete/ConfigManager.h"
#include "../../../Header/Manager/Concrete/AudioManager.h"
#include "../../../Header/Tower/Concrete/Archer.h"
#include "../../../Header/Tower/Concrete/Axeman.h"
#include "../../../Header/Tower/Concrete/Gunner.h"

TowerManager::~TowerManager()
{
	for (Tower* _tower : towerList)
		delete _tower;
}

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
	#pragma region Instantiate
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
		break;
	}
	if (_tower == nullptr)
		return;
	#pragma endregion

	#pragma region Locate
	//利用传入的离散点坐标，获取瓦片地图中对应的瓦片位置，将防御塔实际定位在该位置上
	static Vector2 _position;
	static const SDL_Rect& _mapRect = ProcessManager::Instance()->mapRect;
	_position.x = _mapRect.x + _point.x * TILE_SIZE + TILE_SIZE / 2;
	_position.y = _mapRect.y + _point.y * TILE_SIZE + TILE_SIZE / 2;
	_tower->SetPosition(_position);

	//std::cout << "MapRectXY=(" << _mapRect.x << "," << _mapRect.y << ")\n";
	//std::cout << "PlacePoint=(" << _point.x << "," << _point.y << ")\n";
	//std::cout << "TowerPosition=(" << _position.x << "," << _position.y << ")\n";
	#pragma endregion

	//将实例化的防御塔对象存入列表
	towerList.push_back(_tower);

	//调用地图方法标记特定瓦片处已经存在防御塔
	ProcessManager::Instance()->map.MarkTowerExistenceAt(_point);

	//播放放置防御塔的音效
	AudioManager::Instance()->PlaySFX(SoundResID::Tower_Place);
}

void TowerManager::UpgradeTower(TowerType _type)
{
	static ProcessManager* _process = ProcessManager::Instance();

	//最高级为十级
	switch (_type)
	{
	case TowerType::Archer:
		_process->IncreaseTowerLevelBy(TowerType::Archer, 1);
		break;
	case TowerType::Axeman:
		_process->IncreaseTowerLevelBy(TowerType::Axeman, 1);
		break;
	case TowerType::Gunner:
		_process->IncreaseTowerLevelBy(TowerType::Gunner, 1);
		break;
	default:
		break;
	}

	//播放升级防御塔的音效
	AudioManager::Instance()->PlaySFX(SoundResID::Tower_Upgrade);
}

double TowerManager::GetBuildCostOf(TowerType _type) const
{
	static ConfigManager* _config = ConfigManager::Instance();
	static ProcessManager* _process = ProcessManager::Instance();

	switch (_type)
	{
	case TowerType::Archer:
		return _config->archerPrefab.buildCost[_process->GetTowerLevel(TowerType::Archer)];
		break;
	case TowerType::Axeman:
		return _config->axemanPrefab.buildCost[_process->GetTowerLevel(TowerType::Axeman)];
		break;
	case TowerType::Gunner:
		return _config->gunnerPrefab.buildCost[_process->GetTowerLevel(TowerType::Gunner)];
		break;
	default:
		return -1;
	}
}

double TowerManager::GetUpgradeCostOf(TowerType _type) const
{
	static ConfigManager* _config = ConfigManager::Instance();
	static ProcessManager* _process = ProcessManager::Instance();

	//若防御塔等级超过最大值（即十级，索引为9），应当返回一个无意义值如-1表示无法继续升级，而不是返回有意义值（无法判断是否到达最大值）
	switch (_type)
	{
	case TowerType::Archer:
		return (_process->GetTowerLevel(TowerType::Archer) == 9) ? -1 : 
			_config->archerPrefab.upgradeCost[_process->GetTowerLevel(TowerType::Archer)];
		break;
	case TowerType::Axeman:
		return (_process->GetTowerLevel(TowerType::Axeman) == 9) ? -1 :
			_config->axemanPrefab.upgradeCost[_process->GetTowerLevel(TowerType::Axeman)];
		break;
	case TowerType::Gunner:
		return (_process->GetTowerLevel(TowerType::Gunner) == 9) ? -1 :
			_config->gunnerPrefab.upgradeCost[_process->GetTowerLevel(TowerType::Gunner)];
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
		return _config->archerPrefab.cooldown[_process->GetTowerLevel(TowerType::Archer)];
		break;
	case TowerType::Axeman:
		return _config->axemanPrefab.cooldown[_process->GetTowerLevel(TowerType::Axeman)];
		break;
	case TowerType::Gunner:
		return _config->gunnerPrefab.cooldown[_process->GetTowerLevel(TowerType::Gunner)];
		break;
	default:
		return -1;
	}
}

double TowerManager::GetFireRadiusOf(TowerType _type) const
{
	static ConfigManager* _config = ConfigManager::Instance();
	static ProcessManager* _process = ProcessManager::Instance();

	//此处半径的大小的定义是以瓦片格为单位的，所以要乘上瓦片尺寸
	switch (_type)
	{
	case TowerType::Archer:
		return _config->archerPrefab.viewRange[_process->GetTowerLevel(TowerType::Archer)] * TILE_SIZE;
		break;
	case TowerType::Axeman:
		return _config->axemanPrefab.viewRange[_process->GetTowerLevel(TowerType::Axeman)] * TILE_SIZE;
		break;
	case TowerType::Gunner:
		return _config->gunnerPrefab.viewRange[_process->GetTowerLevel(TowerType::Gunner)] * TILE_SIZE;
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
		return _config->archerPrefab.damage[_process->GetTowerLevel(TowerType::Archer)];
		break;
	case TowerType::Axeman:
		return _config->axemanPrefab.damage[_process->GetTowerLevel(TowerType::Axeman)];
		break;
	case TowerType::Gunner:
		return _config->gunnerPrefab.damage[_process->GetTowerLevel(TowerType::Gunner)];
		break;
	default:
		return -1;
	}
}
