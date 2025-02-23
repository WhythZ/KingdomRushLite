#ifndef _STATUS_UI_H_
#define _STATUS_UI_H_

#include <SDL.h>
//�����й���UI����Ⱦ��ֱ�������ص�Ϊ��λ���ж�λ�ģ�����ʹ��SDL_Point�������������ά�������������漰����������
//#include "../Infra/Vector2.h"

//����ȾԪ�ؼ�ļ������
#define STATUSUI_HOME_RIGHT_DISTANCE 15          //��ͼ�����Ҳ����ȾԪ�أ��׸�Ѫ��ͼ������ͼ�꣩�����Ҽ��
#define STATUSUI_HEALTH_ICON_BETWEEN_DISTANCE 3  //ÿ��Ѫ��ͼ������Ҽ��
#define STATUSUI_COIN_ICON_NUMTEXT_DISTANCE 5    //���ͼ�����������ı������Ҽ��
#define STATUSUI_HOME_PLAYER_ICON_DISTANCE 15    //��ͼ�������ͼ�������¼��

//״̬��UI����һ���Ǽ�ͷ��+��������ֵ��Ѫ��ͼ�꣨ʵʱ��+���ͼ��+�����ֵ��ʵʱ�����ڶ��������ͷ��+��Ҽ�����ȴ����ʵʱ��
class StatusUI
{
private:
	SDL_Point position = { 0, 0 };                                 //�������϶��������λ��

	#pragma region CoinNumText
	SDL_Texture* coinNumTextTexture = nullptr;                     //�ݴ� ��������ı�ת����������Ⱦ����
	SDL_Point coinNumTextSize = { 0, 0 };                          //�ݴ����ı���Ⱦ�Ŀ���볤��

	const SDL_Color coinTextColor = { 255,255,255,255 };           //��������ı��Ĺ̶���ɫ������ɫ��
	#pragma endregion

	#pragma region PlayerMpBar
	//double mpBarRatio = 0;                                         //��ҲٿصĽ�ɫ����ֵ����ʵʱռ��

	const SDL_Point mpBarSize = { 200, 20 };                       //����ֵ��������ߴ�
	const int mpBarBorderThickness = 3;                            //����ֵ���߿�ĺ��
	const SDL_Color mpBarContentForeColor = { 145,120,170,255 };   //����ֵ��������ɫ
	const SDL_Color mpBarContentBackColor = { 50,40,50,255 };      //����ֵ�����ݱ�����ɫ
	#pragma endregion

public:
	StatusUI() = default;
	~StatusUI() = default;
	void SetPosition(SDL_Point);

	void OnUpdate(SDL_Renderer*);                                  //�����ı���������Ⱦ���ݣ����贫����Ⱦ��
	void OnRender(SDL_Renderer*);
};

#endif
