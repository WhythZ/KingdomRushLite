#include "../../../Header/UI/TowerPanel/TowerPanel.h"
#include <string>
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header/Manager/Concrete/UIManager.h"
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
	centerPosition = { _idx.x * TILE_SIZE + TILE_SIZE / 2,_idx.y * TILE_SIZE + TILE_SIZE / 2 };
}

void TowerPanel::OnInput(const SDL_Event& _event)
{
	if (!isActive) return;

	switch (_event.type)
	{
	case SDL_MOUSEMOTION:
	//局部代码块，内部用return跳出
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
	//局部代码块，内部用return跳出
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
		ClosePanel();
	}
		break;
	default:
		break;
	}
}

void TowerPanel::OnUpdate(SDL_Renderer* _renderer)
{
	#pragma region Clear
	//由于每帧OnUpdate函数都会生成一张文本的纹理，故先清除掉上一帧的遗留垃圾
	SDL_DestroyTexture(costTextTexture);
	costTextTexture = nullptr;
	#pragma endregion

	#pragma region GetCostValueString
	//拿到花费数量
	double _cost = -1;
	switch (hoveredButtonType)
	{
	case TowerPanel::ButtonType::None:
		//如果未选中按钮，则
		_cost = 0;
		break;
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
	//若小于零则说明当前花费无效（例如防御塔处于上限等级时无法继续升级），若等于0则什么都不显示
	std::string _costStr = "";
	if (_cost < 0)
		_costStr = "MAX";
	else if (_cost > 0)
		_costStr = std::to_string((int)_cost); //先转浮点为整型防止丑陋小数，再转化为字符串
	else
		_costStr = " "; //注意是一个空格，否则会由于生成的文本纹理尺寸为0而报错
	#pragma endregion

	#pragma region CostTextTexture
	//先将文本以特定字体加载到内存中
	static TTF_Font* _font = ResourceManager::Instance()->GetFontPool().find(FontResID::Ipix)->second;
	SDL_Surface* _costTextSurface = TTF_RenderText_Blended(_font, _costStr.c_str(), costTextColor);
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
	if (!isActive) return;

	//引入纹理渲染相关方法
	static UIManager* _ui = UIManager::Instance();

	//复用的左上顶点坐标
	static SDL_Point _positionLeftUp = { 0,0 };

	#pragma region SelectedTileCursor
	//因为选中光标是包裹着瓦片，所以尺寸与瓦片一致
	static const SDL_Point _selectCursorSize = { TILE_SIZE, TILE_SIZE };
	_positionLeftUp.x = centerPosition.x - TILE_SIZE / 2;
	_positionLeftUp.y = centerPosition.y - TILE_SIZE / 2;
	_ui->DrawTexture(_renderer, TextureResID::UI_SelectCursor, _positionLeftUp, _selectCursorSize);
	#pragma endregion

	#pragma region Panel
	//依据悬停位置绘制对应的轮盘
	_positionLeftUp.x = centerPosition.x - panelSize.x / 2;
	_positionLeftUp.y = centerPosition.y - panelSize.y / 2;
	switch (hoveredButtonType)
	{
	case TowerPanel::ButtonType::None:
		_ui->DrawTexture(_renderer, idlePanelTexture, _positionLeftUp, panelSize);
		break;
	case TowerPanel::ButtonType::Top:
		_ui->DrawTexture(_renderer, topPanelTexture, _positionLeftUp, panelSize);
		break;
	case TowerPanel::ButtonType::Left:
		_ui->DrawTexture(_renderer, leftPanelTexture, _positionLeftUp, panelSize);
		break;
	case TowerPanel::ButtonType::Right:
		_ui->DrawTexture(_renderer, rightPanelTexture, _positionLeftUp, panelSize);
		break;
	default:
		break;
	}
	#pragma endregion

	#pragma region CostText
	//绘制花费文本，在整个轮盘的正中下方
	_positionLeftUp.x = centerPosition.x - costTextSize.x / 2;
	_positionLeftUp.y = centerPosition.y - costTextSize.y / 2 + panelSize.y / 2;
	_ui->DrawTexture(_renderer, costTextTexture, _positionLeftUp, costTextSize);
	#pragma endregion
}

bool TowerPanel::IsActive() const
{
	return isActive;
}

SDL_Point TowerPanel::GetSelectedTileIdx() const
{
	//若当前面板未被显示，则返回一个无意义值，防止返回上一次该面板显示时的对应值
	if (isActive)
		return selectedTileIdx;
	else
		return { -1,-1 };
}

void TowerPanel::ShowPanel()
{
	isActive = true;
}

void TowerPanel::ClosePanel()
{
	isActive = false;
}