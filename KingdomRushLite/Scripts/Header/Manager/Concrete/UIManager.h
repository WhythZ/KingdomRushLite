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
	StatusUI* statusUI;                          //状态栏UI，显示血量和法力值等

	TowerBuildPanel* towerBuildPanel;            //建造防御塔的轮盘UI
	TowerUpgradePanel* towerUpgradePanel;        //升级防御塔的轮盘UI

	GameOverUI* gameOverUI;                      //游戏结束时的UI

public:
	void OnInput(const SDL_Event&);
	void OnUpdate(SDL_Renderer*);
	void OnRender(SDL_Renderer*);

	//根据左上顶点位置、纹理尺寸，绘制纹理图片
	void DrawTexture(SDL_Renderer* , SDL_Texture*, const SDL_Point&, const SDL_Point&);
	void DrawTexture(SDL_Renderer*, TextureResID, const SDL_Point&, const SDL_Point&);
	//根据左上顶点位置、条尺寸、边框厚度、背景颜色、内容颜色、内容数值比例，绘制动态更新的数值条
	void DrawDynamicBar(SDL_Renderer*, const SDL_Point&, const SDL_Point&, int, const SDL_Color&, const SDL_Color&, double);
	//根据圆心位置、圆半径、边框颜色、内容颜色，绘制带边框的实心圆
	void DrawFilledCircle(SDL_Renderer*, const SDL_Point&, double, const SDL_Color&, const SDL_Color&);
	//根据左上顶点位置、矩形尺寸、矩形颜色，绘制一个矩形实心色块
	void DrawFilledBox(SDL_Renderer*, const SDL_Point&, const SDL_Point&, const SDL_Color&);

private:
	UIManager();
	~UIManager();

	bool IsTowerPanelActive() const;             //监测当前场景中是否正在显示防御塔相关轮盘UI
};

#endif
