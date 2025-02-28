#ifndef _PLAYER_DRAGON_H_
#define _PLAYER_DRAGON_H_

#include "../Player.h"

class PlayerDragon :public Player
{
private:
	bool isReleaseFlashSkill = false;
	bool isReleaseImpactSkill = false;

	#pragma region SkillSize
	SDL_Point flashSkillSize = { 300,68 };     //���ڼ��ܣ��������������ĸ����򣬴˴��ߴ�����ˮƽʱ�Ŀ��
	SDL_Point impactSkillSize = { 60,140 };

	SDL_Rect flashSkillRect = { 0 };           //ʵʱ��¼������ЧͼƬ����ײ���������ײ�˺�
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