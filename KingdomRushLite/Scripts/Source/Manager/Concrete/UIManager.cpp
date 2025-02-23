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
	//只有在防御塔轮盘不活跃时，才能触发选择其它的瓦片
	if (!IsTowerPanelActive())
	{
		switch (_event.type)
		{
		case SDL_MOUSEBUTTONUP:
		{
			//获取当前鼠标对应位置的瓦片
			SDL_Point _selectedTileIdx = GameManager::Instance()->GetCursorTileIdx();
			static Map& _map = ProcessManager::Instance()->map;

			//若选中的目标瓦片位置是家，则显示升级UI
			if (_selectedTileIdx.x == _map.GetHomeIdx().x && _selectedTileIdx.y == _map.GetHomeIdx().y)
			{
				//先设置位置，然后再显示面板在对应位置
				towerUpgradePanel->SetSelectedTileIdx(_selectedTileIdx);
				towerUpgradePanel->ShowPanel();
			}
			//若不是家，且不存在防御塔、不是路径，则可以显示建造UI
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
		//由于第一个if语句区域内的代码会使用防御塔轮盘的活跃状态来决定是否监测输入，且由于按下了一个轮盘的按钮后就会关闭轮盘
		//故若将直接如下调用放在第一个if语句区域内的代码前方，会导致按下任意按钮后立刻在对应瓦片出立刻显示另一个轮盘，体验很差
		//若直接放在后方，会导致点击瓦片后对应UI被打开然后迅速关闭（因为被轮盘的输入监测函数内部捕获到鼠标松开）
		//所以此处我们将二者放在了else区域内就解决了上述两个问题
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