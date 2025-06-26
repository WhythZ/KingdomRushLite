#ifndef _UI_MANAGER_H_
#define _UI_MANAGER_H_

#include "../Manager.hpp"
#include <SDL.h>
#include "ResourceManager.h"
#include "../../UI/StatusUI.h"
#include "../../UI/TowerPanel/Concrete/TowerBuildPanel.h"
#include "../../UI/TowerPanel/Concrete/TowerUpgradePanel.h"
#include "../../UI/GameOverUI.h"

class UIManager :public Manager<UIManager>
{
	friend class Manager<UIManager>;

private:
	StatusUI* statusUI;                          //״̬��UI����ʾѪ���ͷ���ֵ��

	TowerBuildPanel* towerBuildPanel;            //���������������UI
	TowerUpgradePanel* towerUpgradePanel;        //����������������UI

	GameOverUI* gameOverUI;                      //��Ϸ����ʱ��UI

public:
	void OnInput(const SDL_Event&);
	void OnUpdate(SDL_Renderer*);
	void OnRender(SDL_Renderer*);

	//�������϶���λ�á�����ߴ磬��������ͼƬ
	void DrawTexture(SDL_Renderer* , SDL_Texture*, const SDL_Point&, const SDL_Point&);
	void DrawTexture(SDL_Renderer*, TextureResID, const SDL_Point&, const SDL_Point&);
	//�������϶���λ�á����ߴ硢�߿��ȡ�������ɫ��������ɫ��������ֵ���������ƶ�̬���µ���ֵ��
	void DrawDynamicBar(SDL_Renderer*, const SDL_Point&, const SDL_Point&, int, const SDL_Color&, const SDL_Color&, double);
	//����Բ��λ�á�Բ�뾶���߿���ɫ��������ɫ�����ƴ��߿��ʵ��Բ
	void DrawFilledCircle(SDL_Renderer*, const SDL_Point&, double, const SDL_Color&, const SDL_Color&);
	//�������϶���λ�á����γߴ硢������ɫ������һ������ʵ��ɫ��
	void DrawFilledBox(SDL_Renderer*, const SDL_Point&, const SDL_Point&, const SDL_Color&);

private:
	UIManager();
	~UIManager();

	bool IsTowerPanelActive() const;             //��⵱ǰ�������Ƿ�������ʾ�������������UI
};

#endif
