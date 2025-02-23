#include "../../../Header/Manager/Concrete/UIManager.h"
#include <iostream>
#include "../../../Header/Manager/Concrete/GameManager.h"
#include "../../../Header/Manager/Concrete/ProcessManager.h"

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
	#pragma region ShowTowerPanel
	//ֻ���ڷ��������̲���Ծʱ�����ܴ���ѡ����������Ƭ
	if (!IsTowerPanelActive())
	{
		switch (_event.type)
		{
		case SDL_MOUSEBUTTONUP:
		{
			//��ȡ��ǰ����Ӧλ�õ���Ƭ
			SDL_Point _selectedTileIdx = GameManager::Instance()->GetCursorTileIdx();
			static Map& _map = ProcessManager::Instance()->map;

			//��ѡ�е�Ŀ����Ƭλ���Ǽң�����ʾ����UI
			if (_selectedTileIdx.x == _map.GetHomeIdx().x && _selectedTileIdx.y == _map.GetHomeIdx().y)
			{
				//������λ�ã�Ȼ������ʾ����ڶ�Ӧλ��
				towerUpgradePanel->SetSelectedTileIdx(_selectedTileIdx);
				towerUpgradePanel->ShowPanel();
			}
			//�����Ǽң��Ҳ����ڷ�����������·�����������ʾ����UI
			else
			{
				if (!_map.IsTowerBuiltAt(_selectedTileIdx) && !_map.IsInRouteAt(_selectedTileIdx))
				{
					towerBuildPanel->SetSelectedTileIdx(_selectedTileIdx);
					towerBuildPanel->ShowPanel();
				}
			}
		}
			break;
		default:
			break;
		}
	}
	else
	{
		//���ڵ�һ��if��������ڵĴ����ʹ�÷��������̵Ļ�Ծ״̬�������Ƿ������룬�����ڰ�����һ�����̵İ�ť��ͻ�ر�����
		//������ֱ�����µ��÷��ڵ�һ��if��������ڵĴ���ǰ�����ᵼ�°������ⰴť�������ڶ�Ӧ��Ƭ��������ʾ��һ�����̣�����ܲ�
		//��ֱ�ӷ��ں󷽣��ᵼ�µ����Ƭ���ӦUI����Ȼ��Ѹ�ٹرգ���Ϊ�����̵������⺯���ڲ���������ɿ���
		//���Դ˴����ǽ����߷�����else�����ھͽ����������������
		towerBuildPanel->OnInput(_event);
		towerUpgradePanel->OnInput(_event);
	}
	#pragma endregion
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

bool UIManager::IsTowerPanelActive() const
{
	return (towerBuildPanel->IsActive() || towerUpgradePanel->IsActive());
}