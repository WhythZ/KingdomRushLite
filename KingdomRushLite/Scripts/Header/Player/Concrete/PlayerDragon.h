#ifndef _PLAYER_DRAGON_H_
#define _PLAYER_DRAGON_H_

#include "../Player.h"

class PlayerDragon :public Player
{
private:
	bool isReleaseFlashSkill = false;
	bool isReleaseImpactSkill = false;

	#pragma region SkillSize
	SDL_Point flashSkillSize = { 300,68 };     //由于技能（长方形纹理）有四个方向，此处尺寸描述水平时的宽高
	SDL_Point impactSkillSize = { 60,140 };

	SDL_Rect flashSkillRect = { 0 };           //实时记录技能特效图片的碰撞体积用于碰撞伤害
	SDL_Rect impactSkillRect = { 0 };
	#pragma endregion

	#pragma region SkillAnimation
	Animation* flashSkillAnimCurrent;
	Animation flashSkillAnimUp;
	Animation flashSkillAnimDown;
	Animation flashSkillAnimLeft;
	Animation flashSkillAnimRight;

	Animation* impactSkillAnimCurrent;
	Animation impactSkillAnimUp;
	Animation impactSkillAnimDown;
	Animation impactSkillAnimLeft;
	Animation impactSkillAnimRight;
	#pragma endregion

public:
	PlayerDragon();
	~PlayerDragon() = default;

	void OnUpdate(double) override;
	void OnRender(SDL_Renderer*) override;

private:
	void TryReleaseSkill00() override;
	void TryReleaseSkill01() override;
};

#endif