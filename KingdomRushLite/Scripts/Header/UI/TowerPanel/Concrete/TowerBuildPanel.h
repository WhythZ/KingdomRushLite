#ifndef _TOWER_BUILD_PANEL_H_
#define _TOWER_BUILD_PANEL_H_

#include "../TowerPanel.h"

class TowerBuildPanel :public TowerPanel
{
private:
	int fireRadiusTowerTop = 0;                           //��ͣ��Ӧ��ťʱ���ƶ�Ӧ�������Ĺ�����Χ
	int fireRadiusTowerLeft = 0;
	int fireRadiusTowerRight = 0;

	SDL_Color fireCircleFrameColor = { 30,50,160,180 };   //������ΧԲ�ı߿���ɫ����һ��͸����
	SDL_Color fireCircleContentColor = { 0,149,217,180 }; //������ΧԲ�������ɫ����һ��͸����

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