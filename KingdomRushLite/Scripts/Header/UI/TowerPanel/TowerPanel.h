#ifndef _TOWER_PANEL_H_
#define _TOWER_PANEL_H_

#include <SDL.h>
//�����ص�Ϊ��λ���ж�λ�������Ϊ��������������ʹ�ø���������
//#include "../Infra/Vector2.h"

//�����������̺�������������
class TowerPanel
{
protected:
	bool isVisible = false;                                 //������ǰ����UI�Ƿ�ɼ�
	SDL_Point centerPosition = { 0,0 };                     //����UI���ĵ����������λ��
	SDL_Point selectedTileIdx;                              //��ǰ����UI��ʾ�ڵ���Ƭ��������

	#pragma region Textures
	SDL_Texture* idleTexture = nullptr;                     //�ĸ���ť��ûѡ��ʱ��������ͻ���κΰ�ť
	SDL_Texture* topTexture = nullptr;                      //ѡ���������Ϸ���ť������ͻ��
	SDL_Texture* leftTexture = nullptr;                     //ѡ���������·���ť������ͻ��
	SDL_Texture* rightTexture = nullptr;                    //ѡ���������·���ť������ͻ��
	#pragma endregion

private:
	#pragma region Panel
	const SDL_Point panelSize = { 144,144 };                //���̵�����ߴ�
	#pragma endregion
	
	#pragma region Button
	const SDL_Point buttonSize = { 48,48 };                 //�����и�����ť�ĳߴ�
	const SDL_Point topButtonOffset = { 48,6 };             //���Ϸ���ťƫ���������϶����ƫ����
	const SDL_Point leftButtonOffset = { 8,80 };            //���·���ťƫ���������϶����ƫ����
	const SDL_Point rightButtonOffset = { 90,80 };          //���·���ťƫ���������϶����ƫ����
	#pragma endregion

	#pragma region Text
	SDL_Point textSize = { 0,0 };
	SDL_Texture* textTexture = nullptr;

	const SDL_Color textColor = { 255,255 ,255 ,255 };
	#pragma endregion

public:
	TowerPanel() = default;
	~TowerPanel() = default;
	void SetPosition(SDL_Point);
};

#endif
