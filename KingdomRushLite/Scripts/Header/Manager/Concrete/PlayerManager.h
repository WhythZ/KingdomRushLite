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
	Player* player = nullptr;                    //ָ��ǰ�����е���ҽ�ɫ
	PlayerType playerType = PlayerType::None;    //��ǰ��������ҽ�ɫ������

public:
	void SetPlayerType(PlayerType);              //�趨��ҽ�ɫ����

	void OnInput(const SDL_Event&);
	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	void InstantiatePlayer();                    //�趨�����ͺ��ڼҵ�λ��ʵ�������

	PlayerType GetPlayerType() const;
	const Vector2& GetPlayerSize() const;
	const Vector2& GetPlayerPosition() const;

private:
	PlayerManager() = default;
	~PlayerManager();
};

#endif