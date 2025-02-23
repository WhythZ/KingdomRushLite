#ifndef _PROCESS_MANAGER_H_
#define _PROCESS_MANAGER_H_

#include "../Manager.hpp"
#include "../../Map/Map.h"
#include "../../Tower/TowerType.h"

//依据配置文件设置各类防御塔的等级上限
#define TOWER_ARCHER_MAX_LEVEL 9
#define TOWER_AXEMAN_MAX_LEVEL 9
#define TOWER_GUNNER_MAX_LEVEL 9

//记录一局游戏中的各项动态属性
class ProcessManager :public Manager<ProcessManager>
{
	friend class Manager<ProcessManager>;

public:
	bool isGameOver = false;                   //波次全部结束，则游戏结束
	bool isWin = true;                         //家没被攻破，游戏就没有输

	#pragma region Map
	Map map;                                   //游戏地图
	SDL_Rect mapRect = { 0 };                  //地图矩形渲染在游戏窗口内的位置与长宽
	#pragma endregion

private:
	#pragma region Home
	double healthMaximum;                      //记录家的最大血量
	double healthCurrent;                      //记录家的当前血量
	#pragma endregion

	#pragma region Coin
	double coinNum = 1000;                        //玩家持有的金币数量
	#pragma endregion

	#pragma region Tower
	int levelArcher = 0;                       //弓箭塔等级（全局统一升级）
	int levelAxeman = 0;                       //投斧手等级
	int levelGunner = 0;                       //枪炮手等级
	#pragma endregion

public:
	double GetCurrentHealth() const;           //获取当前家血量
	void DecreaseHealthBy(double);             //减少当前家血量

	double GetCurrentCoinNum() const;          //获取当前金币数
	void IncreaseCoinNumBy(double);            //增加金币数
	void DecreaseCoinNumBy(double);            //减少金币数

	int GetTowerLevel(TowerType) const;        //获取对应防御塔等级
	void IncreaseTowerLevelBy(TowerType, int); //增加对应防御塔等级

private:
	ProcessManager();
	~ProcessManager() = default;
};

#endif