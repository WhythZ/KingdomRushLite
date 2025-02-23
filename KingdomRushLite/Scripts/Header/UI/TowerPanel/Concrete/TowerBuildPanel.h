#ifndef _TOWER_BUILD_PANEL_H_
#define _TOWER_BUILD_PANEL_H_

#include "../TowerPanel.h"

class TowerBuildPanel :public TowerPanel
{
public:
	TowerBuildPanel() = default;
	~TowerBuildPanel() = default;

	void OnUpdate(SDL_Renderer*) override;
	void OnRender(SDL_Renderer*) override;

protected:
	void OnClickTop() override;
	void OnClickLeft() override;
	void OnClickRight() override;
};

#endif