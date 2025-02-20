#ifndef _STATUS_UI_H_
#define _STATUS_UI_H_

#include <SDL.h>
#include "../Infra/Vector2.h"

class StatusUI
{
private:
	SDL_Point position = { 0 };                                //���϶��������λ��

	#pragma region Text
	SDL_Texture* coinTextTexture = nullptr;                    //�ݴ� ��������ı�ת����������Ⱦ����
	int coinTextWidth = 0;                                     //�ݴ����ı���Ⱦ�Ŀ��
	int coinTextHeight = 0;                                    //�ݴ����ı���Ⱦ�ĳ���

	const SDL_Color coinTextColor = { 255,255,255,255 };       //��������ı��Ĺ̶���ɫ������ɫ��
	#pragma endregion

	#pragma region Bar
	const Vector2 mpBarSize = { 200, 20 };                     //��ҲٿصĽ�ɫ�ķ���ֵ���ĳߴ�
	const int mpBarBorderThickness = 3;                        //����ֵ���߿�ĺ��
	const SDL_Color mpBarContentColor = { 145,120,170,255 };   //����ֵ��������ɫ
	const SDL_Color mpBarContentBackColor = { 50,40,50,255 };  //����ֵ�����ݱ�����ɫ
	#pragma endregion

	#pragma region Icon
	const Vector2 hpIconSize = { 32,32 };                      //Ѫ����ֵ�ı���ͼ��ĳߴ�
	#pragma endregion

public:
	StatusUI() = default;
	~StatusUI() = default;
	void SetPosition(SDL_Point);

	void OnUpdate(double, SDL_Renderer*);                      //�����ı���������Ⱦ���ݣ����贫����Ⱦ��
	void OnRender(SDL_Renderer*);
};

#endif
