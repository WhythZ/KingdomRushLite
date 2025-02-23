#include "../../../../Header/UI/TowerPanel/Concrete/TowerBuildPanel.h"
#include <SDL2_gfxPrimitives.h>
#include "../../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../../Header/Manager/Concrete/ProcessManager.h"
#include "../../../../Header/Manager/Concrete/AudioManager.h"
#include "../../../../Header/Manager/Concrete/TowerManager.h"

TowerBuildPanel::TowerBuildPanel()
{
	static const TexturePool& _texPool = ResourceManager::Instance()->GetTexturePool();

	idlePanelTexture = _texPool.find(TextureResID::UI_Build_Idle)->second;
	topPanelTexture = _texPool.find(TextureResID::UI_Build_HoveredTop)->second;
	leftPanelTexture = _texPool.find(TextureResID::UI_Build_HoveredLeft)->second;
	rightPanelTexture = _texPool.find(TextureResID::UI_Build_HoveredRight)->second;
}

void TowerBuildPanel::OnUpdate(SDL_Renderer* _renderer)
{
	static TowerManager* _tm = TowerManager::Instance();

	#pragma region UpdateCost
	//更新对应位置按钮的花费值，素材中正上方位置是投斧手，左下方是弓箭手，右下方是枪炮手
	topCostValue = _tm->GetBuildCostOf(TowerType::Axeman);
	leftCostValue = _tm->GetBuildCostOf(TowerType::Archer);
	rightCostValue = _tm->GetBuildCostOf(TowerType::Gunner);
	#pragma endregion

	//父类的更新函数中使用到了三个位置按钮的花费变量，所以要在变量更新后再调用
	TowerPanel::OnUpdate(_renderer);

	#pragma region UpdateFireRadius
	//更新对应位置按钮的攻击范围半径，用于范围圆的绘制
	fireRadiusTowerTop = _tm->GetFireRadiusOf(TowerType::Axeman);
	fireRadiusTowerLeft = _tm->GetFireRadiusOf(TowerType::Archer);
	fireRadiusTowerRight = _tm->GetFireRadiusOf(TowerType::Gunner);
	#pragma endregion
}

void TowerBuildPanel::OnRender(SDL_Renderer* _renderer)
{
	#pragma region FireCircle
	//绘制攻击范围圆
	double _radius = 0;
	switch (hoveredButtonType)
	{
	case TowerPanel::ButtonType::Top:
		_radius = fireRadiusTowerTop;
		break;
	case TowerPanel::ButtonType::Left:
		_radius = fireRadiusTowerLeft;
		break;
	case TowerPanel::ButtonType::Right:
		_radius = fireRadiusTowerRight;
		break;
	default:
		break;
	}

	//若半径有效且允许显示，则绘制攻击范围圆
	if (isShowFireCircle && _radius > 0)
	{
		//绘制圆填充
		filledCircleRGBA(_renderer, centerPosition.x, centerPosition.y, (Sint16)_radius,
			fireCircleContentColor.r, fireCircleContentColor.g, fireCircleContentColor.b, fireCircleContentColor.a);
		//绘制圆边框
		aacircleRGBA(_renderer, centerPosition.x, centerPosition.y, (Sint16)_radius,
			fireCircleFrameColor.r, fireCircleFrameColor.g, fireCircleFrameColor.b, fireCircleFrameColor.a);
	}
	#pragma endregion

	//在攻击范围圆后渲染，防止前者遮挡原本UI
	TowerPanel::OnRender(_renderer);
}

void TowerBuildPanel::ShowPanel()
{
	TowerPanel::ShowPanel();

	//同时打开攻击范围圆的显示许可
	isShowFireCircle = true;
}

void TowerBuildPanel::ClosePanel()
{
	TowerPanel::ClosePanel();

	//同时关闭攻击范围圆的显示许可
	isShowFireCircle = false;
}

void TowerBuildPanel::OnClickTop()
{
	ProcessManager* _pm = ProcessManager::Instance();

	//首先检测当前金币数量是否足够建造对应种类的防御塔
	if (topCostValue <= _pm->GetCurrentCoinNum())
	{
		//扣除对应数额金币，然后实例化防御塔
		_pm->DecreaseCoinNumBy(topCostValue);
		TowerManager::Instance()->BuildTower(TowerType::Axeman, selectedTileIdx);

		//播放对应建造音效
		AudioManager::Instance()->PlaySFX(SoundResID::Tower_Build);
	}
	//否则播放操作失败音效
	else
		AudioManager::Instance()->PlaySFX(SoundResID::Tower_Error);
}

void TowerBuildPanel::OnClickLeft()
{
	ProcessManager* _pm = ProcessManager::Instance();

	if (leftCostValue <= _pm->GetCurrentCoinNum())
	{
		_pm->DecreaseCoinNumBy(leftCostValue);
		TowerManager::Instance()->BuildTower(TowerType::Archer, selectedTileIdx);

		AudioManager::Instance()->PlaySFX(SoundResID::Tower_Build);
	}
	else
		AudioManager::Instance()->PlaySFX(SoundResID::Tower_Error);
}

void TowerBuildPanel::OnClickRight()
{
	ProcessManager* _pm = ProcessManager::Instance();

	if (rightCostValue <= _pm->GetCurrentCoinNum())
	{
		_pm->DecreaseCoinNumBy(rightCostValue);
		TowerManager::Instance()->BuildTower(TowerType::Gunner, selectedTileIdx);

		AudioManager::Instance()->PlaySFX(SoundResID::Tower_Build);
	}
	else
		AudioManager::Instance()->PlaySFX(SoundResID::Tower_Error);
}