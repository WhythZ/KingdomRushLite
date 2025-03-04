#include "../../../Header/Manager/Concrete/ProcessManager.h"
#include "../../../Header/Manager/Concrete/ConfigManager.h"
#include "../../../Header/Manager/Concrete/AudioManager.h"

ProcessManager::ProcessManager()
{
	//Ѫ����ȡ
	healthMaximum = ConfigManager::Instance()->initHomeHP;
	healthCurrent = healthMaximum;
}

double ProcessManager::GetCurrentHealth() const
{
	return healthCurrent;
}

void ProcessManager::DecreaseHealthBy(double _decre)
{
	//����Ѫ��
	healthCurrent -= _decre;
	if (healthCurrent <= 0)
	{
		healthCurrent = 0;
		//��Ϸ������ʧ��
		isGameOver = true;
		isWin = false;
	}

	//�����ܻ���Ч
	AudioManager::Instance()->PlaySFX(SoundResID::Home_Hurt);
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

int ProcessManager::GetTowerLevel(TowerType _type) const
{
	int _val = -1;

	switch (_type)
	{
	case TowerType::Archer:
		_val = levelArcher;
		break;
	case TowerType::Axeman:
		_val = levelAxeman;
		break;
	case TowerType::Gunner:
		_val = levelGunner;
		break;
	default:
		_val = -1;
		break;
	}

	if (_val < 0)
		std::cout << "Method \"int ProcessManager::GetTowerLevel(TowerType)\" returned a meaningless negative value";

	return _val;
}

void ProcessManager::IncreaseTowerLevelBy(TowerType _type, int _incre)
{
	switch (_type)
	{
	case TowerType::Archer:
		//��1����ʼ���TOWER_MAX_LEVEL���𣬶�Ӧ�������������(TOWER_MAX_LEVEL - 1)
		levelArcher = (levelArcher + _incre >= TOWER_MAX_LEVEL - 1) ? (TOWER_MAX_LEVEL - 1) : (levelArcher + _incre);
		break;
	case TowerType::Axeman:
		levelAxeman = (levelAxeman + _incre >= TOWER_MAX_LEVEL - 1) ? (TOWER_MAX_LEVEL - 1) : (levelAxeman + _incre);
		break;
	case TowerType::Gunner:
		levelGunner = (levelGunner + _incre >= TOWER_MAX_LEVEL - 1) ? (TOWER_MAX_LEVEL - 1) : (levelGunner + _incre);
		break;
	default:
		break;
	}
}