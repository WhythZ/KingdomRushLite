#include "../../../Header/Player/Concrete/PlayerDragon.h"

PlayerDragon::PlayerDragon()
{

}

void PlayerDragon::OnInput(const SDL_Event& _event)
{
	Player::OnInput(_event);
}

void PlayerDragon::OnUpdate(double _delta)
{
	Player::OnUpdate(_delta);
}

void PlayerDragon::OnRender(SDL_Renderer* _renderer)
{
	Player::OnRender(_renderer);
}

void PlayerDragon::ReleaseSkill00()
{

}

void PlayerDragon::ReleaseSkill01()
{

}