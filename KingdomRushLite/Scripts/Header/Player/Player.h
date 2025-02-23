#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "../Infra/Vector2.h"
#include "PlayerType.h"
#include "../Infra/Animation.h"
#include "../Infra/DirectionType.h"

class Player
{
protected:
	PlayerType type = PlayerType::None;

	FacingDir direction = FacingDir::Down;

	#pragma region Animation
	Animation animIdleUp;
	Animation animIdleDown;
	Animation animIdleLeft;
	Animation animIdleRight;
	#pragma endregion

private:
	Vector2 size;
	Vector2 position;
	Vector2 velocity;

	Animation* animCurrent;

public:
	Player() = default;
	~Player() = default;
	void SetPosition(const Vector2&);

	virtual void OnInput(const SDL_Event&);
	virtual void OnUpdate(double);
	virtual void OnRender(SDL_Renderer*);
};

#endif