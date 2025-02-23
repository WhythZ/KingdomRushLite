#ifndef _TOWER_UPGRADE_PANEL_H_
#define _TOWER_UPGRADE_PANEL_H_

#include "../TowerPanel.h"

class TowerUpgradePanel :public TowerPanel
{
public:
	TowerUpgradePanel() = default;
	~TowerUpgradePanel() = default;

	void OnUpdate(SDL_Renderer*) override;
	void OnRender(SDL_Renderer*) override;

protected:
	void OnClickTop() override;
	void OnClickLeft() override;
	void OnClickRight() override;
};

#endif