#ifndef _PLAYER_MANAGER_H_
#define _PLAYER_MANAGER_H_

#include "../Manager.hpp"
#include <SDL.h>
#include "../../Player/Player.h"
#include "../../Player/PlayerType.h"

class PlayerManager :public Manager<PlayerManager>
{
	friend class Manager<PlayerManager>;

public:
	Player* player = nullptr;                    //指向当前场景中的玩家角色

public:
	void OnInput(const SDL_Event&);
	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	void InstantiatePlayer(PlayerType);          //在家的位置实例化特定类型的玩家

private:
	PlayerManager() = default;
	~PlayerManager();
};

#endif