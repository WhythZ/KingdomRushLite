#include "../../../Header/Manager/Concrete/ProcessManager.h"
#include "../../../Header/Manager/Concrete/ConfigManager.h"
#include "../../../Header/Manager/Concrete/AudioManager.h"

ProcessManager::ProcessManager()
{
	//血量读取
	healthMaximum = ConfigManager::Instance()->initHomeHP;
	healthCurrent = healthMaximum;
}

double ProcessManager::GetCurrentHealth() const
{
	return healthCurrent;
}

void ProcessManager::DecreaseHealthBy(double _decre)
{
	//减少血量
	healthCurrent -= _decre;
	if (healthCurrent <= 0)
		healthCurrent = 0;

	//播放受击音效
	AudioManager::Instance()->PlaySFX(SoundResID::HomeHurt);
}

double ProcessManager::GetCurrentCoinNum() const
{
	return coinNum;
}

void ProcessManager::IncreaseCoinNumBy(double _reward)
{
	coinNum += _reward;
}

void ProcessManager::DecreaseCoinNumBy(double _cost)
{
	coinNum = (coinNum - _cost > 0) ? (coinNum - _cost) : 0;
}

int ProcessManager::GetTowerLevel(TowerType _type)
{
	switch (_type)
	{
	case TowerType::Archer:
		return levelArcher;
	case TowerType::Axeman:
		return levelAxeman;
	case TowerType::Gunner:
		return levelGunner;
	default:
		break;
	}
}

void ProcessManager::IncreaseTowerLevelBy(TowerType _type, int _incre)
{
	switch (_type)
	{
	case TowerType::Archer:
		levelArcher = (levelArcher + _incre >= TOWER_ARCHER_MAX_LEVEL) ? TOWER_ARCHER_MAX_LEVEL : (levelArcher + _incre);
		break;
	case TowerType::Axeman:
		levelAxeman = (levelAxeman + _incre >= TOWER_AXEMAN_MAX_LEVEL) ? TOWER_AXEMAN_MAX_LEVEL : (levelAxeman + _incre);
		break;
	case TowerType::Gunner:
		levelGunner = (levelGunner + _incre >= TOWER_GUNNER_MAX_LEVEL) ? TOWER_GUNNER_MAX_LEVEL : (levelGunner + _incre);
		break;
	default:
		break;
	}
}