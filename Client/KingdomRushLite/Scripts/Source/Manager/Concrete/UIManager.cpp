#include "../../../Header/Manager/Concrete/UIManager.h"
#include <iostream>
#include <SDL2_gfxPrimitives.h>
#include "../../../Header/Manager/Concrete/GameManager.h"
#include "../../../Header/Manager/Concrete/ProcessManager.h"

UIManager::UIManager()
{
	statusUI = new StatusUI();

	towerBuildPanel = new TowerBuildPanel();
	towerUpgradePanel = new TowerUpgradePanel();

	gameOverUI = new GameOverUI();
}

UIManager::~UIManager()
{
	delete statusUI;

	delete towerBuildPanel;
	delete towerUpgradePanel;

	delete gameOverUI;
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
			//若不是家，且不存在防御塔、不是路径、不存在装饰，则可以显示建造UI
			else
			{
				if (!_map.IsTowerBuiltAt(_selectedTileIdx)
					&& !_map.IsInRouteAt(_selectedTileIdx)
					&& !_map.IsDecorationAt(_selectedTileIdx))
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
	static ProcessManager* _pm = ProcessManager::Instance();
	if (!_pm->isGameOver)
	{
		statusUI->OnUpdate(_renderer);
		towerBuildPanel->OnUpdate(_renderer);
		towerUpgradePanel->OnUpdate(_renderer);
	}
	else
		gameOverUI->OnUpdate(_renderer);
}

void UIManager::OnRender(SDL_Renderer* _renderer)
{
	static ProcessManager* _pm = ProcessManager::Instance();
	if (!_pm->isGameOver)
	{
		statusUI->OnRender(_renderer);
		towerBuildPanel->OnRender(_renderer);
		towerUpgradePanel->OnRender(_renderer);
	}
	else
		gameOverUI->OnRender(_renderer);
}

void UIManager::DrawTexture(SDL_Renderer* _renderer, SDL_Texture* _texture, const SDL_Point& _leftUpPosition, const SDL_Point& _size)
{
	//临时存储每个渲染元素，每次渲染单个元素时复用
	static SDL_Rect _dstRect;

	//根据传入的顶点与尺寸绘制纹理
	_dstRect = { _leftUpPosition.x, _leftUpPosition.y, _size.x, _size.y };
	SDL_RenderCopy(_renderer, _texture, nullptr, &_dstRect);
}

void UIManager::DrawTexture(SDL_Renderer* _renderer, TextureResID _id, const SDL_Point& _leftUpPosition, const SDL_Point& _size)
{
	//获取纹理图片资源
	static const TexturePool& _texPool = ResourceManager::Instance()->GetTexturePool();
	//这个纹理指针不能用static修饰，否则会导致所有调用此函数所绘制出的图形均为第一个调用此函数时的图像
	SDL_Texture* _texture = _texPool.find(_id)->second;

	static SDL_Rect _dstRect;

	_dstRect = { _leftUpPosition.x, _leftUpPosition.y, _size.x, _size.y };
	SDL_RenderCopy(_renderer, _texture, nullptr, &_dstRect);
}

void UIManager::DrawDynamicBar(SDL_Renderer* _renderer, const SDL_Point& _leftUpPosition, const SDL_Point& _size, int _borderThickness,
	const SDL_Color& _backgroundColor, const SDL_Color& _contentColor, double _contentRatio)
{
	static SDL_Rect _dstRect;

	//先规整传入的比例，控制在[0,1]之间
	double _ratio = (_contentRatio < 0) ? 0 : _contentRatio;
	_ratio = (_ratio > 1) ? 1 : _ratio;

	//数值值条先绘制背景颜色填充，再绘制内容颜色填充，先传入渲染器与左上顶点和右下顶点，然后是颜色
	_dstRect = { _leftUpPosition.x, _leftUpPosition.y, _size.x, _size.y };
	boxRGBA(_renderer, _dstRect.x, _dstRect.y, _dstRect.x + _dstRect.w, _dstRect.y + _dstRect.h,
		_backgroundColor.r, _backgroundColor.g, _backgroundColor.b, _backgroundColor.a);
	//如果是如下绘制圆角矩形，则还需传入圆角半径
	//roundedBoxRGBA(_renderer, _dstRect.x, _dstRect.y, _dstRect.x + _dstRect.w, _dstRect.y + _dstRect.h,
	//	4, _backgroundColor.r, _backgroundColor.g, _backgroundColor.b, _backgroundColor.a);

	//绘制内容的填充矩形（其水平宽度按照比例实时更新），其依据边框宽度收窄，背景颜色就相当于在内容颜色的外围形成一圈边框
	_dstRect = { _leftUpPosition.x + _borderThickness, _leftUpPosition.y + _borderThickness,
		(int)((_size.x - 2 * _borderThickness) * _ratio), _size.y - 2 * _borderThickness };
	boxRGBA(_renderer, _dstRect.x, _dstRect.y, _dstRect.x + _dstRect.w, _dstRect.y + _dstRect.h,
		_contentColor.r, _contentColor.g, _contentColor.b, _contentColor.a);
	//roundedBoxRGBA(_renderer, _dstRect.x, _dstRect.y, _dstRect.x + _dstRect.w, _dstRect.y + _dstRect.h,
	//	4, _contentColor.r, _contentColor.g, _contentColor.b, _contentColor.a);
}

void UIManager::DrawFilledCircle(SDL_Renderer* _renderer, const SDL_Point& _leftUpPosition, double _radius,
	const SDL_Color& _borderColor, const SDL_Color& _contentColor)
{
	//绘制圆填充
	filledCircleRGBA(_renderer, _leftUpPosition.x, _leftUpPosition.y, (Sint16)_radius,
		_contentColor.r, _contentColor.g, _contentColor.b, _contentColor.a);
	//绘制圆边框
	aacircleRGBA(_renderer, _leftUpPosition.x, _leftUpPosition.y, (Sint16)_radius,
		_borderColor.r, _borderColor.g, _borderColor.b, _borderColor.a);
}

void UIManager::DrawFilledBox(SDL_Renderer* _renderer, const SDL_Point& _leftUpPosition, const SDL_Point& _size, const SDL_Color& _color)
{
	boxRGBA(_renderer, _leftUpPosition.x, _leftUpPosition.y, _leftUpPosition.x + _size.x, _leftUpPosition.y + _size.y,
		_color.r, _color.g, _color.b, _color.a);
}

bool UIManager::IsTowerPanelActive() const
{
	return (towerBuildPanel->IsActive() || towerUpgradePanel->IsActive());
}