#ifndef _PLAYER_MANAGER_H_
#define _PLAYER_MANAGER_H_

#include "../Manager.hpp"
#include <SDL.h>
#include "../../Player/Player.h"
#include "../../Player/PlayerType.h"

class PlayerManager :public Manager<PlayerManager>
{
	friend class Manager<PlayerManager>;

private:
	Player* player = nullptr;                    //指向当前场景中的玩家角色
	PlayerType playerType = PlayerType::None;    //当前场景中玩家角色的类型

public:
	void SetPlayerType(PlayerType);              //设定玩家角色类型

	void OnInput(const SDL_Event&);
	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	void InstantiatePlayer();                    //设定好类型后，在家的位置实例化玩家

	PlayerType GetPlayerType() const;

private:
	PlayerManager() = default;
	~PlayerManager();
};

#endif