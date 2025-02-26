#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "PlayerType.h"
#include "../Infra/Vector2.h"
#include "../Infra/DirectionType.h"
#include "../Infra/StateMachine/StateMachine.h"
#include "State/Concrete/PlayerIdleState.h"
#include "State/Concrete/PlayerMoveState.h"
#include "State/Concrete/PlayerSkillState.h"

class Player
{
public:
	FacingDir facingDir = FacingDir::Down;

	double xInput = 0;
	double yInput = 0;

protected:
	PlayerType type = PlayerType::None;
	Vector2 size;
	Vector2 position;
	Vector2 velocity;

	#pragma region Stats
	double speed = 0;
	double skill00Cooldown = 10;
	double skill00Damage = 0;
	double skill01Cooldown = 10;
	double skill01Damage = 0;
	#pragma endregion

	#pragma region State
	StateMachine* stateMachine;

	bool isIdle = true;
	PlayerIdleState* idleState;
	
	//通过四个布尔值决定isMove的值
	bool isMoveUp = false;
	bool isMoveDown = false;
	bool isMoveLeft = false;
	bool isMoveRight = false;
	bool isMove = false;
	PlayerMoveState* moveState;

	bool isSkill = false;
	PlayerSkillState* skillState;
	#pragma endregion

public:
	Player();
	~Player();
	void SetPosition(const Vector2&);
	void SetVelocity(const Vector2&);

	void OnInput(const SDL_Event&);
	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	PlayerType GetType() const;
	const Vector2& GetSize() const;
	const Vector2& GetPosition() const;
	double GetSpeed() const;

protected:
	virtual void ReleaseSkill00() = 0;
	virtual void ReleaseSkill01() = 0;
};

#endif