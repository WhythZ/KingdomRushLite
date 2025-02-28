#ifndef _PLAYER_DRAGON_H_
#define _PLAYER_DRAGON_H_

#include "../Player.h"

class PlayerDragon :public Player
{
private:
	double impactSkillSlowDownDuration = 2;    //震慑技能暂停敌人移动的持续时间
	SDL_Point flashSkillSize = { 300,68 };     //由于技能（长方形纹理）有四个方向，此处尺寸描述水平时的宽高
	SDL_Point impactSkillSize = { 60,140 };

	#pragma region SkillBool
	//由于技能碰撞需要在技能范围Rect更新后使用，因为Rect并未在按下技能按键后立刻更新
	//所以不能直接在按下按键后的技能释放时直接调用碰撞，否则会导致每次释放技能时的碰撞Rect都是上一次释放技能时的那个
	bool canFlashSkillCollide = false;
	bool canImpactSkillCollide = false;

	bool isReleaseFlashSkill = false;
	bool isReleaseImpactSkill = false;
	#pragma endregion

	#pragma region SkillRect
	SDL_Rect flashSkillAnimRect = { 0 };       //实时记录技能特效图片的渲染区域用于动画渲染
	SDL_Rect impactSkillAnimRect = { 0 };

	SDL_Rect flashSkillCollideRect = { 0 };    //实时记录技能的攻击范围区域用于碰撞检测
	SDL_Rect impactSkillCollideRect = { 0 };
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

	void OnFlashCollide();                     //在释放技能的瞬间对范围内的敌人造成伤害
	void OnImpactCollide();
};

#endif