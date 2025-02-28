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
	#pragma region State
	StateMachine* stateMachine;
	
	PlayerIdleState* idleState;
	PlayerMoveState* moveState;
	PlayerSkillState* skillState;
	#pragma endregion

	FacingDir facingDir = FacingDir::Down;

	double xInput = 0;
	double yInput = 0;

protected:
	PlayerType type = PlayerType::None;
	Vector2 size;
	Vector2 position;
	Vector2 velocity;

	#pragma region StateBool
	bool isIdle = true;
	bool isMove = false;
	bool isSkill = false;
	#pragma endregion

	#pragma region Stats
	double speed = 0;
	double skill00Cooldown = 10;
	double skill00Damage = 0;
	double skill01Cooldown = 10;
	double skill01Damage = 0;
	#pragma endregion

	#pragma region Skill
	bool canReleaseSkill00 = true;
	bool canReleaseSkill01 = true;

	Timer skill00Timer;
	Timer skill01Timer;
	#pragma endregion

public:
	Player();
	~Player();
	void SetPosition(const Vector2&);
	void SetVelocity(const Vector2&);

	void OnInput(const SDL_Event&);
	virtual void OnUpdate(double);
	virtual void OnRender(SDL_Renderer*);

	void HandleMovementInput(const SDL_Event&);      //OnInput函数并非在每个状态都处理用户输入
	void HandleSkillInput(const SDL_Event&);

	PlayerType GetType() const;
	const Vector2& GetSize() const;
	const Vector2& GetPosition() const;
	double GetSpeed() const;

protected:
	virtual void TryReleaseSkill00() = 0;
	virtual void TryReleaseSkill01() = 0;
};

#endif