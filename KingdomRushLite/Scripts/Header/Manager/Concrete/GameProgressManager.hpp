#ifndef _GAME_PROGRESS_MANAGER_HPP_
#define _GAME_PROGRESS_MANAGER_HPP_

#include "../Manager.hpp"

class GameProgressManager :public Manager<GameProgressManager>
{
	friend Manager<GameProgressManager>;

public:
	bool isGameOver = false;             //����ȫ������������Ϸ����
	bool isWin = true;                   //��û�����ƣ���Ϸ��û����

private:
	GameProgressManager() = default;
	~GameProgressManager() = default;
};

#endif