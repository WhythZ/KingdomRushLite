#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "../Infra/Vector2.h"
#include "PlayerType.h"
#include "../Infra/StateMachine/StateMachine.h"
#include "../Infra/DirectionType.h"


class Player
{
protected:
	PlayerType type = PlayerType::None;

	#pragma region State

	bool isMovingUp = true;
	bool isMovingDown = true;
	bool isMovingLeft = true;
	bool isMovingRight = true;
	#pragma endregion

private:
	Vector2 size;
	Vector2 position;
	Vector2 velocity;

	FacingDir direction = FacingDir::Down;

	bool isIdling = true;
	bool isSkilling = false;

public:
	Player();
	~Player();
	void SetPosition(const Vector2&);

	virtual void OnInput(const SDL_Event&);
	virtual void OnUpdate(double);
	virtual void OnRender(SDL_Renderer*);

protected:
	virtual void ReleaseSkill00() = 0;
	virtual void ReleaseSkill01() = 0;
};

#endif