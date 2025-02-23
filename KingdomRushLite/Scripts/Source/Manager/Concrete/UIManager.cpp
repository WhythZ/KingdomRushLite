#include "../../../Header/Manager/Concrete/UIManager.h"

UIManager::UIManager()
{
	statusUI = new StatusUI();

	towerBuildPanel = new TowerBuildPanel();
	towerUpgradePanel = new TowerUpgradePanel();
}

UIManager::~UIManager()
{
	delete statusUI;
	delete towerBuildPanel;
	delete towerUpgradePanel;
}

void UIManager::OnInput(const SDL_Event& _event)
{
	towerBuildPanel->OnInput(_event);
	towerUpgradePanel->OnInput(_event);
}

void UIManager::OnUpdate(SDL_Renderer* _renderer)
{
	statusUI->OnUpdate(_renderer);

	towerBuildPanel->OnUpdate(_renderer);
	towerUpgradePanel->OnUpdate(_renderer);
}

void UIManager::OnRender(SDL_Renderer* _renderer)
{
	statusUI->OnRender(_renderer);

	towerBuildPanel->OnRender(_renderer);
	towerUpgradePanel->OnRender(_renderer);
}