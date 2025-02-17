#ifndef _PROCESS_MANAGER_HPP_
#define _PROCESS_MANAGER_HPP_

#include "../Manager.hpp"
#include "ConfigManager.hpp"
#include "AudioManager.hpp"
#include "../../Map/Map.hpp"

//记录一局游戏中的各项动态属性
class ProcessManager :public Manager<ProcessManager>
{
	friend Manager<ProcessManager>;

public:
	bool isGameOver = false;             //波次全部结束，则游戏结束
	bool isWin = true;                   //家没被攻破，游戏就没有输

	#pragma region Map
	Map map;                             //游戏地图
	SDL_Rect mapRect = { 0 };            //地图矩形渲染在游戏窗口内的位置与长宽
	#pragma endregion

	#pragma region Tower
	int levelArcher = 0;                 //弓箭塔等级（全局统一升级）
	int levelAxeman = 0;                 //投斧手等级
	int levelGunner = 0;                 //枪炮手等级
	#pragma endregion

	#pragma region Coin
	double coinNum = 0;                  //玩家持有的金币数量
	#pragma endregion

private:
	#pragma region Home
	double healthMaximum;                //记录家的最大血量
	double healthCurrent;                //记录家的当前血量
	#pragma endregion

public:
	void IncreaseCoinNumBy(double);      //增加金币数
	void DecreaseCoinNumBy(double);      //减少金币数

	double GetCurrentHealth() const;     //获取当前家血量
	void DecreaseHealthBy(double);       //减少当前家血量

private:
	ProcessManager();
	~ProcessManager() = default;
};

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

void ProcessManager::IncreaseCoinNumBy(double _reward)
{
	coinNum += _reward;
}

void ProcessManager::DecreaseCoinNumBy(double _cost)
{
	coinNum = (coinNum - _cost > 0) ? (coinNum - _cost) : 0;
}

#endif