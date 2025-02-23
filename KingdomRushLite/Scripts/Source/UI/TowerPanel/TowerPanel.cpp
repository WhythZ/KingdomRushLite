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

	//�̶�������Ƭ������ȡ����λ����������
	centerPosition = { _idx.x * TILE_SIZE,_idx.y * TILE_SIZE };
}

void TowerPanel::OnInput(const SDL_Event& _event)
{
	if (!isVisible) return;

	switch (_event.type)
	{
	case SDL_MOUSEMOTION:
	{
		//��ȡʵʱ���λ�ã���������Ƿ���Ŀ�갴ť��Χ��
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

		//��δ�������ⰴť�����ڣ���δѡ��
		hoveredButtonType = ButtonType::None;
	}
		break;
	case SDL_MOUSEBUTTONUP:
	{
		//���¼�Ϊ���İ���̧�𣨼������޷�Ӧ����̧��ʱ�������൱�ڵ��һ�Σ���������Ӧ�������
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

		//��ɵ����ȡ��������ʾ
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
	//����ÿ֡OnUpdate������������һ���ı������������������һ֡����������
	SDL_DestroyTexture(costTextTexture);
	costTextTexture = nullptr;
	#pragma endregion

	#pragma region CostValidity
	//�õ���������
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
	//���Ϸ��Ժ���ת��Ϊ�ַ�������С������˵����ǰ������Ч������������������޵ȼ�ʱ�޷���������
	std::string _coinNumStr = (_cost < 0) ? "MAX" : std::to_string(rightCostValue);
	#pragma endregion

	#pragma region CostText
	//�Ƚ��ı����ض�������ص��ڴ���
	static TTF_Font* _font = ResourceManager::Instance()->GetFontPool().find(FontResID::Pixel_CN)->second;
	SDL_Surface* _costTextSurface = TTF_RenderText_Blended(_font, _coinNumStr.c_str(), costTextColor);
	//��ȡת�����ͼƬ�ĳ���
	costTextSize = { _costTextSurface->w, _costTextSurface->h };
	//Ȼ���ٽ���ת��Ϊ�����ʽ
	costTextTexture = SDL_CreateTextureFromSurface(_renderer, _costTextSurface);
	//Ȼ�������Ѿ������˵�Surface����
	SDL_FreeSurface(_costTextSurface);
	#pragma endregion
}

void TowerPanel::OnRender(SDL_Renderer* _renderer)
{
	if (!isVisible) return;

	//��ʱ�洢ÿ����ȾԪ�أ�ÿ����Ⱦ����Ԫ��ʱ����
	static SDL_Rect _dstRect;

	#pragma region SelectedTileCursor
	//��Ϊѡ�й���ǰ�������Ƭ�����Գߴ�����Ƭһ��
	_dstRect.x = centerPosition.x - TILE_SIZE / 2;
	_dstRect.y = centerPosition.y - TILE_SIZE / 2;
	_dstRect.w = TILE_SIZE;
	_dstRect.h = TILE_SIZE;
	SDL_RenderCopy(_renderer, selectedTileCursorTexture, nullptr, &_dstRect);
	#pragma endregion

	#pragma region Panel
	//������ͣλ�û��ƶ�Ӧ������
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
	//���ƻ����ı������������̵������·�
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