#ifndef _UI_MANAGER_H_
#define _UI_MANAGER_H_

#include "../Manager.hpp"
#include <SDL.h>
#include "../../UI/StatusUI.h"
#include "../../UI/TowerPanel/Concrete/TowerBuildPanel.h"
#include "../../UI/TowerPanel/Concrete/TowerUpgradePanel.h"

class UIManager :public Manager<UIManager>
{
	friend class Manager<UIManager>;

private:
	StatusUI* statusUI;                     //״̬��UI���ڵ�ǰ��������ʼ��ʱʵ����

	TowerBuildPanel* towerBuildPanel;
	TowerUpgradePanel* towerUpgradePanel;

public:
	void OnInput(const SDL_Event&);
	void OnUpdate(SDL_Renderer*);
	void OnRender(SDL_Renderer*);

private:
	UIManager();
	~UIManager();
};

#endif
