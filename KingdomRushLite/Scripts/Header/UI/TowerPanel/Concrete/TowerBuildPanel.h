#ifndef _TOWER_BUILD_PANEL_H_
#define _TOWER_BUILD_PANEL_H_

#include "../TowerPanel.h"

class TowerBuildPanel :public TowerPanel
{
private:
	bool isShowFireCircle = false;                           //��ͣ��Ӧ��ťʱ���ƶ�Ӧ�������Ĺ�����Χ
	double fireRadiusTowerTop = 0;
	double fireRadiusTowerLeft = 0;
	double fireRadiusTowerRight = 0;

	SDL_Color fireCircleFrameColor = { 30,50,160,180 };      //������ΧԲ�ı߿���ɫ����һ��͸����
	SDL_Color fireCircleContentColor = { 0,149,217,180 };    //������ΧԲ�������ɫ����һ��͸����

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