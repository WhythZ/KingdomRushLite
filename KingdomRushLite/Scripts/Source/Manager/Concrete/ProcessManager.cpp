#include "../../../Header/Manager/Concrete/ProcessManager.h"
#include "../../../Header/Manager/Concrete/ConfigManager.h"
#include "../../../Header/Manager/Concrete/AudioManager.h"

ProcessManager::ProcessManager()
{
	//Ѫ����ȡ
	healthMaximum = ConfigManager::Instance()->initHomeHP;
	healthCurrent = healthMaximum;
}

void ProcessManager::DecreaseHealthBy(double _decre)
{
	//����Ѫ��
	healthCurrent -= _decre;
	if (healthCurrent <= 0)
		healthCurrent = 0;

	//�����ܻ���Ч
	AudioManager::Instance()->PlaySFX(SoundResID::HomeHurt);
}

void ProcessManager::IncreaseCoinNumBy(double _reward)
{
	coinNum += _reward;
}

void ProcessManager::DecreaseCoinNumBy(double _cost)
{
	coinNum = (coinNum - _cost > 0) ? (coinNum - _cost) : 0;
}

double ProcessManager::GetCurrentHealth() const
{
	return healthCurrent;
}

double ProcessManager::GetCurrentCoin() const
{
	return coinNum;
}