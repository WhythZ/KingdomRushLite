#ifndef _TOWER_PANEL_H_
#define _TOWER_PANEL_H_

#include <SDL.h>
//�����ص�Ϊ��λ���ж�λ�������Ϊ��������������ʹ�ø���������
//#include "../Infra/Vector2.h"

//�����������̺�������������
class TowerPanel
{
protected:
	enum class ButtonType
	{
		None,
		Top,
		Left,
		Right
	};

protected:
	bool isVisible = false;                                 //������ǰ����UI�Ƿ�ɼ�
	SDL_Point selectedTileIdx = { 0,0 };                    //��ǰ����UI��ʾ�ڵ���Ƭ��������
	SDL_Point centerPosition = { 0,0 };                     //����UI���ĵ����������λ��

	int topCostValue = 0;                                   //ʵʱ��¼���Ϸ���ť��Ӧ����������
	int leftCostValue = 0;                                  //ʵʱ��¼���·���ť��Ӧ����������
	int rightCostValue = 0;                                 //ʵʱ��¼���·���ť��Ӧ����������

	ButtonType hoveredButtonType = ButtonType::None;        //��ͣѡ�еİ�ť��ö��

	#pragma region Texture
	SDL_Texture* selectedTileCursorTexture = nullptr;       //ѡ��ĳ����Ƭʱ�Ĺ��

	SDL_Texture* idlePanelTexture = nullptr;                //�ĸ���ť��ûѡ��ʱ��������ͻ���κΰ�ť
	SDL_Texture* topPanelTexture = nullptr;                 //ѡ���������Ϸ���ť������ͻ��
	SDL_Texture* leftPanelTexture = nullptr;                //ѡ���������·���ť������ͻ��
	SDL_Texture* rightPanelTexture = nullptr;               //ѡ���������·���ť������ͻ��
	#pragma endregion

private:
	const SDL_Point panelSize = { 144,144 };                //���̵�����ߴ�
	
	#pragma region Button
	const SDL_Point buttonSize = { 48,48 };                 //�����и�����ť�ĳߴ�
	const SDL_Point topButtonOffset = { 48,6 };             //���Ϸ���ťƫ���������϶����ƫ����
	const SDL_Point leftButtonOffset = { 8,80 };            //���·���ťƫ���������϶����ƫ����
	const SDL_Point rightButtonOffset = { 90,80 };          //���·���ťƫ���������϶����ƫ����
	#pragma endregion

	#pragma region CostText
	SDL_Point costTextSize = { 0,0 };
	SDL_Texture* costTextTexture = nullptr;
	const SDL_Color costTextColor = { 255,255 ,255 ,255 };
	#pragma endregion

public:
	TowerPanel();
	~TowerPanel();
	void SetSelectedTileIdx(const SDL_Point&);              //��ȡѡ����Ƭ���꣬ͬʱ��ȡ��λ��

	void OnInput(const SDL_Event&);                         //�������λ������
	virtual void OnUpdate(SDL_Renderer*);                   //�����ı���������Ⱦ���ݣ����贫����Ⱦ��
	virtual void OnRender(SDL_Renderer*);

	void ShowPanel();                                       //��ʾ������

protected:
	virtual void OnClickTop() = 0;
	virtual void OnClickLeft() = 0;
	virtual void OnClickRight() = 0;
};

#endif
