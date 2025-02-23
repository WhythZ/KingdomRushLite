#include "../../../Header/Manager/Concrete/PlayerManager.h"
#include "../../../Header/Manager/Concrete/ProcessManager.h"
#include "../../../Header/Player/Concrete/PlayerDragon.h"

PlayerManager::~PlayerManager()
{
	delete player;
}

void PlayerManager::SetPlayerType(PlayerType _type)
{
	playerType = _type;
}

void PlayerManager::OnInput(const SDL_Event& _event)
{
	player->OnInput(_event);
}

void PlayerManager::OnUpdate(double _delta)
{
	player->OnUpdate(_delta);
}

void PlayerManager::OnRender(SDL_Renderer* _renderer)
{
	player->OnRender(_renderer);
}

void PlayerManager::InstantiatePlayer()
{
	switch (playerType)
	{
	case PlayerType::Dragon:
		player = new PlayerDragon();
		break;
	default:
		break;
	}

	//生成在家的位置
	Vector2 _homePosition = {
		ProcessManager::Instance()->map.GetHomeIdx().x * TILE_SIZE,
		ProcessManager::Instance()->map.GetHomeIdx().y * TILE_SIZE
	};
	player->SetPosition(_homePosition);
}