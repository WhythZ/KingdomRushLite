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
	//���¶�Ӧλ�ð�ť�Ļ���ֵ���ز������Ϸ�λ����Ͷ���֣����·��ǹ����֣����·���ǹ����
	topCostValue = _tm->GetUpgradeCostOf(TowerType::Axeman);
	leftCostValue = _tm->GetUpgradeCostOf(TowerType::Archer);
	rightCostValue = _tm->GetUpgradeCostOf(TowerType::Gunner);
	#pragma endregion

	//����ĸ��º�����ʹ�õ�������λ�ð�ť�Ļ��ѱ���������Ҫ�ڱ������º��ٵ���
	TowerPanel::OnUpdate(_renderer);
}

void TowerUpgradePanel::OnRender(SDL_Renderer* _renderer)
{
	TowerPanel::OnRender(_renderer);
}

void TowerUpgradePanel::OnClickTop()
{
	ProcessManager* _pm = ProcessManager::Instance();

	//���ȼ�⵱ǰ��������Ƿ��㹻������Ӧ����ķ�����
	if (topCostValue <= _pm->GetCurrentCoinNum())
	{
		//�۳���Ӧ�����ң�Ȼ��������Ӧ����ķ�����
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