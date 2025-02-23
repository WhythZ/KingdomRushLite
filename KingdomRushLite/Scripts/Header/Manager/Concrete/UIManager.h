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
	StatusUI* statusUI;                     //状态栏UI，在当前管理器初始化时实例化

	TowerBuildPanel* towerBuildPanel;
	TowerUpgradePanel* towerUpgradePanel;

public:
	void OnInput(const SDL_Event&);
	void OnUpdate(SDL_Renderer*);
	void OnRender(SDL_Renderer*);

	bool IsTowerPanelActive() const;        //监测当前场景中是否正在显示防御塔相关轮盘UI

private:
	UIManager();
	~UIManager();
};

#endif
