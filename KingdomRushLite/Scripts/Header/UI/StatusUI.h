#ifndef _STATUS_UI_H_
#define _STATUS_UI_H_

#include <SDL.h>
//�����й���UI����Ⱦ��ֱ�������ص�Ϊ��λ���ж�λ�ģ�����ʹ��SDL_Point�������������ά�������������漰����������
//#include "../Infra/Vector2.h"

//״̬��UI����һ���Ǽ�ͷ��+��������ֵ��Ѫ��ͼ�꣨ʵʱ��+���ͼ��+�����ֵ��ʵʱ�����ڶ��������ͷ��+��Ҽ�����ȴ����ʵʱ��
class StatusUI
{
private:
	#pragma region Setting
	#pragma region Size
	const SDL_Point healthHeartIconSize = { 32,32 };               //Ѫ������ͼ��ߴ�
	const SDL_Point coinIconSize = { 32,32 };                      //���ͼ��ߴ�

	SDL_Point coinNumTextSize = { 0, 0 };                          //�ݴ������ı���Ⱦ�ĳߴ�
	SDL_Point waveNumTextSize = { 0, 0 };                          //�ݴ沨�����ı���Ⱦ�ĳߴ�

	const SDL_Point playerIconSize = { 65,65 };                    //���ͷ��ͼ��ߴ�

	const SDL_Point mpBarSize = { 240, 24 };                       //����ֵ��������ߴ�
	const int mpBarBorderThickness = 3;                            //����ֵ���߿�ĺ��
	#pragma endregion

	#pragma region Distance
	const int healthHeartIconBetweenDistance = 3;                  //ÿ��Ѫ��ͼ������Ҽ��

	const int iconTextBetweenDistance = 5;                         //ͼ�����ı������Ҽ��

	const int rowBetweenDistance = 10;                             //������ȾԪ�ص����¼��

	const int iconBarBetweenDistance = 5;                          //ͼ������ֵ�������Ҽ��
	#pragma endregion

	#pragma region Color
	const SDL_Color coinNumTextColor = { 255,255,255,255 };        //��������ı��Ĺ̶���ɫ
	const SDL_Color waveNumTextColor = { 255,0,0,255 };            //���������ı��Ĺ̶���ɫ

	const SDL_Color mpBarBackgroundColor = { 50,40,50,255 };       //����ֵ�����ݱ�����ɫ
	const SDL_Color mpBarContentColor = { 145,120,170,255 };       //����ֵ��������ɫ
	#pragma endregion
	#pragma endregion

	SDL_Texture* coinNumTextTexture = nullptr;                     //�ݴ��������ı�ת����������Ⱦ����
	SDL_Texture* waveNumTextTexture = nullptr;                     //�ݴ沨�������ı�ת����������Ⱦ����
	
	double skill00MpBarRatio = 1;                                  //��ҲٿصĽ�ɫ����ֵ����ʵʱռ��
	double skill01MpBarRatio = 1;

public:
	StatusUI() = default;
	~StatusUI() = default;

	void OnUpdate(SDL_Renderer*);                                  //�����ı���������Ⱦ���ݣ����贫����Ⱦ��
	void OnRender(SDL_Renderer*);
};

#endif
