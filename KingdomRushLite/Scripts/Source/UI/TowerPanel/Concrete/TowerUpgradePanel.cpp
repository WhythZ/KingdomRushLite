#include "../../../../Header/UI/TowerPanel/Concrete/TowerUpgradePanel.h"
#include "../../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../../Header/Manager/Concrete/ProcessManager.h"
#include "../../../../Header/Manager/Concrete/TowerManager.h"

TowerUpgradePanel::TowerUpgradePanel()
{
	static const TexturePool& _texPool = ResourceManager::Instance()->GetTexturePool();

	idlePanelTexture = _texPool.find(TextureResID::UI_Upgrade_Idle)->second;
	topPanelTexture = _texPool.find(TextureResID::UI_Upgrade_HoveredTop)->second;
	leftPanelTexture = _texPool.find(TextureResID::UI_Upgrade_HoveredLeft)->second;
	rightPanelTexture = _texPool.find(TextureResID::UI_Upgrade_HoveredRight)->second;
}

void TowerUpgradePanel::OnUpdate(SDL_Renderer* _renderer)
{
	static TowerManager* _tm = TowerManager::Instance();

	#pragma region UpdateCost
	//更新对应位置按钮的花费值，素材中正上方位置是投斧手，左下方是弓箭手，右下方是枪炮手
	topCostValue = _tm->GetUpgradeCostOf(TowerType::Axeman);
	leftCostValue = _tm->GetUpgradeCostOf(TowerType::Archer);
	rightCostValue = _tm->GetUpgradeCostOf(TowerType::Gunner);
	#pragma endregion

	//父类的更新函数中使用到了三个位置按钮的花费变量，所以要在变量更新后再调用
	TowerPanel::OnUpdate(_renderer);
}

void TowerUpgradePanel::OnRender(SDL_Renderer* _renderer)
{
	TowerPanel::OnRender(_renderer);
}

void TowerUpgradePanel::OnClickTop()
{
	ProcessManager* _pm = ProcessManager::Instance();

	//首先检测当前金币数量是否足够升级对应种类的防御塔
	if (topCostValue <= _pm->GetCurrentCoinNum())
	{
		//扣除对应数额金币，然后升级对应种类的防御塔
		_pm->DecreaseCoinNumBy(topCostValue);
		TowerManager::Instance()->UpgradeTower(TowerType::Axeman);
	}
}

void TowerUpgradePanel::OnClickLeft()
{
	ProcessManager* _pm = ProcessManager::Instance();

	if (leftCostValue <= _pm->GetCurrentCoinNum())
	{
		_pm->DecreaseCoinNumBy(leftCostValue);
		TowerManager::Instance()->UpgradeTower(TowerType::Archer);
	}
}

void TowerUpgradePanel::OnClickRight()
{
	ProcessManager* _pm = ProcessManager::Instance();

	if (rightCostValue <= _pm->GetCurrentCoinNum())
	{
		_pm->DecreaseCoinNumBy(rightCostValue);
		TowerManager::Instance()->UpgradeTower(TowerType::Gunner);
	}
}