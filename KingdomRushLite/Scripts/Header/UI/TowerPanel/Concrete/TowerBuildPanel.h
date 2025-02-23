#ifndef _TOWER_BUILD_PANEL_H_
#define _TOWER_BUILD_PANEL_H_

#include "../TowerPanel.h"

class TowerBuildPanel :public TowerPanel
{
private:
	int fireRadiusTowerTop = 0;                           //悬停对应按钮时绘制对应防御塔的攻击范围
	int fireRadiusTowerLeft = 0;
	int fireRadiusTowerRight = 0;

	SDL_Color fireCircleFrameColor = { 30,50,160,180 };   //攻击范围圆的边框颜色，有一定透明度
	SDL_Color fireCircleContentColor = { 0,149,217,180 }; //攻击范围圆的填充颜色，有一定透明度

public:
	TowerBuildPanel();
	~TowerBuildPanel() = default;

	void OnUpdate(SDL_Renderer*) override;
	void OnRender(SDL_Renderer*) override;

protected:
	void OnClickTop() override;
	void OnClickLeft() override;
	void OnClickRight() override;
};

#endif