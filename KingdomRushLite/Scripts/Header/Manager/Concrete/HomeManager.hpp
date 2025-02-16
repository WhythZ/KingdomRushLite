#ifndef _HOME_MANAGER_HPP_
#define _HOME_MANAGER_HPP_

#include "../Manager.hpp"
#include "ConfigManager.hpp"
#include "AudioManager.hpp"

class HomeManager :public Manager<HomeManager>
{
	friend Manager<HomeManager>;

private:
	double healthMaximum;              //��¼�ҵ����Ѫ��
	double healthCurrent;              //��¼�ҵĵ�ǰѪ��

public:
	double GetCurrentHealth() const;   //��ȡ��ǰѪ��
	void DecreaseHealthBy(double);     //���ٷ���Ѫ��

private:
	HomeManager();
	~HomeManager() = default;
};

HomeManager::HomeManager()
{
	//Ѫ����ȡ
	healthMaximum = ConfigManager::Instance()->initHomeHP;
	healthCurrent = healthMaximum;
}

double HomeManager::GetCurrentHealth() const
{
	return healthCurrent;
}

void HomeManager::DecreaseHealthBy(double _decre)
{
	//����Ѫ��
	healthCurrent -= _decre;
	if (healthCurrent <= 0)
		healthCurrent = 0;

	//�����ܻ���Ч
	AudioManager::Instance()->PlaySFX(SoundResID::HomeHurt);
}

#endif
