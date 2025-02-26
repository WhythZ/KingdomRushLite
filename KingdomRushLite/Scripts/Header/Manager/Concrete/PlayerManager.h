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
	Player* player = nullptr;                    //ָ��ǰ�����е���ҽ�ɫ

public:
	void OnInput(const SDL_Event&);
	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	void InstantiatePlayer(PlayerType);          //�ڼҵ�λ��ʵ�����ض����͵����

private:
	PlayerManager() = default;
	~PlayerManager();
};

#endif