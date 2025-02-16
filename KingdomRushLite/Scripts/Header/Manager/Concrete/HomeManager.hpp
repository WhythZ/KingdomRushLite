#ifndef _HOME_MANAGER_HPP_
#define _HOME_MANAGER_HPP_

#include "../Manager.hpp"
#include "ConfigManager.hpp"
#include "AudioManager.hpp"

class HomeManager :public Manager<HomeManager>
{
	friend Manager<HomeManager>;

private:
	double healthMaximum;              //记录家的最大血量
	double healthCurrent;              //记录家的当前血量

public:
	double GetCurrentHealth() const;   //获取当前血量
	void DecreaseHealthBy(double);     //减少房屋血量

private:
	HomeManager();
	~HomeManager() = default;
};

HomeManager::HomeManager()
{
	//血量读取
	healthMaximum = ConfigManager::Instance()->initHomeHP;
	healthCurrent = healthMaximum;
}

double HomeManager::GetCurrentHealth() const
{
	return healthCurrent;
}

void HomeManager::DecreaseHealthBy(double _decre)
{
	//减少血量
	healthCurrent -= _decre;
	if (healthCurrent <= 0)
		healthCurrent = 0;

	//播放受击音效
	AudioManager::Instance()->PlaySFX(SoundResID::HomeHurt);
}

#endif
