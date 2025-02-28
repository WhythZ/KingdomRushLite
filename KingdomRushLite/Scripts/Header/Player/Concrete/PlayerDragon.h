#ifndef _PLAYER_DRAGON_H_
#define _PLAYER_DRAGON_H_

#include "../Player.h"

class PlayerDragon :public Player
{
private:
	double impactSkillSlowDownDuration = 2;    //���弼����ͣ�����ƶ��ĳ���ʱ��
	SDL_Point flashSkillSize = { 300,68 };     //���ڼ��ܣ��������������ĸ����򣬴˴��ߴ�����ˮƽʱ�Ŀ��
	SDL_Point impactSkillSize = { 60,140 };

	#pragma region SkillBool
	//���ڼ�����ײ��Ҫ�ڼ��ܷ�ΧRect���º�ʹ�ã���ΪRect��δ�ڰ��¼��ܰ��������̸���
	//���Բ���ֱ���ڰ��°�����ļ����ͷ�ʱֱ�ӵ�����ײ������ᵼ��ÿ���ͷż���ʱ����ײRect������һ���ͷż���ʱ���Ǹ�
	bool canFlashSkillCollide = false;
	bool canImpactSkillCollide = false;

	bool isReleaseFlashSkill = false;
	bool isReleaseImpactSkill = false;
	#pragma endregion

	#pragma region SkillRect
	SDL_Rect flashSkillAnimRect = { 0 };       //ʵʱ��¼������ЧͼƬ����Ⱦ�������ڶ�����Ⱦ
	SDL_Rect impactSkillAnimRect = { 0 };

	SDL_Rect flashSkillCollideRect = { 0 };    //ʵʱ��¼���ܵĹ�����Χ����������ײ���
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

	void OnFlashCollide();                     //���ͷż��ܵ�˲��Է�Χ�ڵĵ�������˺�
	void OnImpactCollide();
};

#endif