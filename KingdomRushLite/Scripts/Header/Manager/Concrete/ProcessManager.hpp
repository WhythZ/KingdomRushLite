#ifndef _PROCESS_MANAGER_HPP_
#define _PROCESS_MANAGER_HPP_

#include "../Manager.hpp"

//记录一局游戏中的各项动态属性
class ProcessManager :public Manager<ProcessManager>
{
	friend Manager<ProcessManager>;

public:
	bool isGameOver = false;             //波次全部结束，则游戏结束
	bool isWin = true;                   //家没被攻破，游戏就没有输

	#pragma region TowerLevel
	int levelArcher = 0;                 //弓箭塔等级（全局统一升级）
	int levelAxeman = 0;                 //投斧手等级
	int levelGunner = 0;                 //枪炮手等级
	#pragma endregion

private:
	ProcessManager() = default;
	~ProcessManager() = default;
};

#endif