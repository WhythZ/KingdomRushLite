#ifndef _TOWER_BUILD_PANEL_H_
#define _TOWER_BUILD_PANEL_H_

#include "../TowerPanel.h"

class TowerBuildPanel :public TowerPanel
{
private:
	#pragma region Setting
	#pragma region Color
	SDL_Color fireCircleBorderColor = { 30,50,160,180 };     //攻击范围圆的边框颜色，有一定透明度
	SDL_Color fireCircleContentColor = { 0,149,217,180 };    //攻击范围圆的填充颜色，有一定透明度
	#pragma endregion
	#pragma endregion

	bool isShowFireCircle = false;                           //悬停对应按钮时绘制对应防御塔的攻击范围

	double fireRadiusTowerTop = 0;
	double fireRadiusTowerLeft = 0;
	double fireRadiusTowerRight = 0;

public:
	TowerBuildPanel();
	~TowerBuildPanel() = default;

	void OnUpdate(SDL_Renderer*) override;
	void OnRender(SDL_Renderer*) override;

	void ShowPanel() override;
	void ClosePanel() override;

protected:
	void OnClickTop() override;
	void OnClickLeft() override;
	void OnClickRight() override;
};

#endif