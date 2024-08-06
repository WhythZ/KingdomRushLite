#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "Manager.h"
class GameManager : public Manager<GameManager>
{
	//授予基类的GetInstance函数权限以调用GameManager类的保护的构造函数
	friend class Manager<GameManager>;
public:
protected:
	GameManager()
	{
	}
	~GameManager()
	{
	}
};

#endif // !_GAME_MANAGER_H_
