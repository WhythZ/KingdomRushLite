#include "../../../../Header/UI/TowerPanel/Concrete/TowerBuildPanel.h"
#include <SDL2_gfxPrimitives.h>
#include "../../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../../Header/Manager/Concrete/ProcessManager.h"
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
	//���¶�Ӧλ�ð�ť�Ļ���ֵ���ز������Ϸ�λ����Ͷ���֣����·��ǹ����֣����·���ǹ����
	topCostValue = _tm->GetBuildCostOf(TowerType::Axeman);
	leftCostValue = _tm->GetBuildCostOf(TowerType::Archer);
	rightCostValue = _tm->GetBuildCostOf(TowerType::Gunner);
	#pragma endregion

	//����ĸ��º�����ʹ�õ�������λ�ð�ť�Ļ��ѱ���������Ҫ�ڱ������º��ٵ���
	TowerPanel::OnUpdate(_renderer);

	#pragma region UpdateFireRadius
	//���¶�Ӧλ�ð�ť�Ĺ�����Χ�뾶�����ڷ�ΧԲ�Ļ���
	fireRadiusTowerTop = _tm->GetFireRadiusOf(TowerType::Axeman);
	fireRadiusTowerLeft = _tm->GetFireRadiusOf(TowerType::Archer);
	fireRadiusTowerRight = _tm->GetFireRadiusOf(TowerType::Gunner);
	#pragma endregion
}

void TowerBuildPanel::OnRender(SDL_Renderer* _renderer)
{
	TowerPanel::OnRender(_renderer);

	#pragma region FireCircle
	//���ƹ�����ΧԲ
	int _radius = 0;
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
	if (_radius > 0)
	{
		//����Բ���
		filledCircleRGBA(_renderer, centerPosition.x, centerPosition.y, _radius,
			fireCircleContentColor.r, fireCircleContentColor.g, fireCircleContentColor.b, fireCircleContentColor.a);
		//����Բ�߿�
		aacircleRGBA(_renderer, centerPosition.x, centerPosition.y, _radius,
			fireCircleFrameColor.r, fireCircleFrameColor.g, fireCircleFrameColor.b, fireCircleFrameColor.a);
	}
	#pragma endregion
}

void TowerBuildPanel::OnClickTop()
{
	ProcessManager* _pm = ProcessManager::Instance();

	//���ȼ�⵱ǰ��������Ƿ��㹻�����Ӧ����ķ�����
	if (topCostValue <= _pm->GetCurrentCoinNum())
	{
		//�۳���Ӧ�����ң�Ȼ��ʵ����������
		_pm->DecreaseCoinNumBy(topCostValue);
		TowerManager::Instance()->BuildTower(TowerType::Axeman, selectedTileIdx);
	}
}

void TowerBuildPanel::OnClickLeft()
{
	ProcessManager* _pm = ProcessManager::Instance();

	if (leftCostValue <= _pm->GetCurrentCoinNum())
	{
		_pm->DecreaseCoinNumBy(leftCostValue);
		TowerManager::Instance()->BuildTower(TowerType::Archer, selectedTileIdx);
	}
}

void TowerBuildPanel::OnClickRight()
{
	ProcessManager* _pm = ProcessManager::Instance();

	if (rightCostValue <= _pm->GetCurrentCoinNum())
	{
		_pm->DecreaseCoinNumBy(rightCostValue);
		TowerManager::Instance()->BuildTower(TowerType::Gunner, selectedTileIdx);
	}
}