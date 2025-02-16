#ifndef _GAME_PROGRESS_MANAGER_HPP_
#define _GAME_PROGRESS_MANAGER_HPP_

#include "../Manager.hpp"

class GameProgressManager :public Manager<GameProgressManager>
{
	friend Manager<GameProgressManager>;

public:
	bool isGameOver = false;             //波次全部结束，则游戏结束
	bool isWin = true;                   //家没被攻破，游戏就没有输

private:
	GameProgressManager() = default;
	~GameProgressManager() = default;
};

#endif