#include "../../../Header/UI/TowerPanel/TowerPanel.h"
#include <string>
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header/Map/Tile.h"

TowerPanel::TowerPanel()
{
	ResourceManager* _res = ResourceManager::Instance();

	selectedTileCursorTexture = _res->GetTexturePool().find(TextureResID::UI_SelectCursor)->second;
}

TowerPanel::~TowerPanel()
{
	SDL_DestroyTexture(costTextTexture);
}

void TowerPanel::SetSelectedTileIdx(const SDL_Point& _idx)
{
	selectedTileIdx = _idx;

	//继而根据瓦片索引获取中心位置像素坐标
	centerPosition = { _idx.x * TILE_SIZE,_idx.y * TILE_SIZE };
}

void TowerPanel::OnInput(const SDL_Event& _event)
{
	if (!isVisible) return;

	switch (_event.type)
	{
	case SDL_MOUSEMOTION:
	{
		//获取实时鼠标位置，并检测其是否处于目标按钮范围内
		SDL_Point _cursorPos = { _event.motion.x, _event.motion.y };
		SDL_Rect _buttonRect = { 0, 0, buttonSize.x, buttonSize.y };
	
		_buttonRect.x = centerPosition.x - panelSize.x / 2 + topButtonOffset.x;
		_buttonRect.y = centerPosition.y - panelSize.y / 2 + topButtonOffset.y;
		if (SDL_PointInRect(&_cursorPos, &_buttonRect))
		{
			hoveredButtonType = ButtonType::Top;
			return;
		}

		_buttonRect.x = centerPosition.x - panelSize.x / 2 + leftButtonOffset.x;
		_buttonRect.y = centerPosition.y - panelSize.y / 2 + leftButtonOffset.y;
		if (SDL_PointInRect(&_cursorPos, &_buttonRect))
		{
			hoveredButtonType = ButtonType::Left;
			return;
		}

		_buttonRect.x = centerPosition.x - panelSize.x / 2 + rightButtonOffset.x;
		_buttonRect.y = centerPosition.y - panelSize.y / 2 + rightButtonOffset.y;
		if (SDL_PointInRect(&_cursorPos, &_buttonRect))
		{
			hoveredButtonType = ButtonType::Right;
			return;
		}

		//若未处于任意按钮区域内，则未选中
		hoveredButtonType = ButtonType::None;
	}
		break;
	case SDL_MOUSEBUTTONUP:
	{
		//该事件为鼠标的按键抬起（即按下无反应，而抬起时触发，相当于点击一次），出发对应点击函数
		switch (hoveredButtonType)
		{
		case TowerPanel::ButtonType::Top:
			OnClickTop();
			break;
		case TowerPanel::ButtonType::Left:
			OnClickLeft();
			break;
		case TowerPanel::ButtonType::Right:
			OnClickRight();
			break;
		default:
			break;
		}

		//完成点击后，取消轮盘显示
		isVisible = false;
	}
		break;
	default:
		break;
	}
}

void TowerPanel::OnUpdate(SDL_Renderer* _renderer)
{
	if (hoveredButtonType == ButtonType::None) return;

	#pragma region Clear
	//由于每帧OnUpdate函数都会生成一张文本的纹理，故先清除掉上一帧的遗留垃圾
	SDL_DestroyTexture(costTextTexture);
	costTextTexture = nullptr;
	#pragma endregion

	#pragma region CostValidity
	//拿到花费数量
	int _cost = -1;
	switch (hoveredButtonType)
	{
	case TowerPanel::ButtonType::Top:
		_cost = topCostValue;
		break;
	case TowerPanel::ButtonType::Left:
		_cost = leftCostValue;
		break;
	case TowerPanel::ButtonType::Right:
		_cost = rightCostValue;
		break;
	default:
		break;
	}
	//检测合法性后再转换为字符串，若小于零则说明当前花费无效，例如防御塔处于上限等级时无法继续升级
	std::string _coinNumStr = (_cost < 0) ? "MAX" : std::to_string(rightCostValue);
	#pragma endregion

	#pragma region CostText
	//先将文本以特定字体加载到内存中
	static TTF_Font* _font = ResourceManager::Instance()->GetFontPool().find(FontResID::Pixel_CN)->second;
	SDL_Surface* _costTextSurface = TTF_RenderText_Blended(_font, _coinNumStr.c_str(), costTextColor);
	//获取转化后的图片的长宽
	costTextSize = { _costTextSurface->w, _costTextSurface->h };
	//然后再将其转化为纹理格式
	costTextTexture = SDL_CreateTextureFromSurface(_renderer, _costTextSurface);
	//然后清理已经无用了的Surface垃圾
	SDL_FreeSurface(_costTextSurface);
	#pragma endregion
}

void TowerPanel::OnRender(SDL_Renderer* _renderer)
{
	if (!isVisible) return;

	//临时存储每个渲染元素，每次渲染单个元素时复用
	static SDL_Rect _dstRect;

	#pragma region SelectedTileCursor
	//因为选中光标是包裹着瓦片，所以尺寸与瓦片一致
	_dstRect.x = centerPosition.x - TILE_SIZE / 2;
	_dstRect.y = centerPosition.y - TILE_SIZE / 2;
	_dstRect.w = TILE_SIZE;
	_dstRect.h = TILE_SIZE;
	SDL_RenderCopy(_renderer, selectedTileCursorTexture, nullptr, &_dstRect);
	#pragma endregion

	#pragma region Panel
	//依据悬停位置绘制对应的轮盘
	_dstRect.x = centerPosition.x - panelSize.x / 2;
	_dstRect.y = centerPosition.y - panelSize.y / 2;
	_dstRect.w = panelSize.x;
	_dstRect.h = panelSize.y;
	switch (hoveredButtonType)
	{
	case TowerPanel::ButtonType::None:
		SDL_RenderCopy(_renderer, idlePanelTexture, nullptr, &_dstRect);
		break;
	case TowerPanel::ButtonType::Top:
		SDL_RenderCopy(_renderer, topPanelTexture, nullptr, &_dstRect);
		break;
	case TowerPanel::ButtonType::Left:
		SDL_RenderCopy(_renderer, leftPanelTexture, nullptr, &_dstRect);
		break;
	case TowerPanel::ButtonType::Right:
		SDL_RenderCopy(_renderer, rightPanelTexture, nullptr, &_dstRect);
		break;
	default:
		break;
	}
	#pragma endregion

	#pragma region CostText
	//绘制花费文本，在整个轮盘的正中下方
	_dstRect.x = centerPosition.x - costTextSize.x / 2;
	_dstRect.y = centerPosition.y - costTextSize.y / 2 + panelSize.y / 2;
	_dstRect.w = costTextSize.x;
	_dstRect.h = costTextSize.y;
	SDL_RenderCopy(_renderer, costTextTexture, nullptr, &_dstRect);
	#pragma endregion
}

void TowerPanel::ShowPanel()
{
	isVisible = true;
}