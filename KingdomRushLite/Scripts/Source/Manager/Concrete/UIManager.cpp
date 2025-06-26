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
			//�����Ǽң��Ҳ����ڷ�����������·����������װ�Σ��������ʾ����UI
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
	//��ʱ�洢ÿ����ȾԪ�أ�ÿ����Ⱦ����Ԫ��ʱ����
	static SDL_Rect _dstRect;

	//���ݴ���Ķ�����ߴ��������
	_dstRect = { _leftUpPosition.x, _leftUpPosition.y, _size.x, _size.y };
	SDL_RenderCopy(_renderer, _texture, nullptr, &_dstRect);
}

void UIManager::DrawTexture(SDL_Renderer* _renderer, TextureResID _id, const SDL_Point& _leftUpPosition, const SDL_Point& _size)
{
	//��ȡ����ͼƬ��Դ
	static const TexturePool& _texPool = ResourceManager::Instance()->GetTexturePool();
	//�������ָ�벻����static���Σ�����ᵼ�����е��ô˺��������Ƴ���ͼ�ξ�Ϊ��һ�����ô˺���ʱ��ͼ��
	SDL_Texture* _texture = _texPool.find(_id)->second;

	static SDL_Rect _dstRect;

	_dstRect = { _leftUpPosition.x, _leftUpPosition.y, _size.x, _size.y };
	SDL_RenderCopy(_renderer, _texture, nullptr, &_dstRect);
}

void UIManager::DrawDynamicBar(SDL_Renderer* _renderer, const SDL_Point& _leftUpPosition, const SDL_Point& _size, int _borderThickness,
	const SDL_Color& _backgroundColor, const SDL_Color& _contentColor, double _contentRatio)
{
	static SDL_Rect _dstRect;

	//�ȹ�������ı�����������[0,1]֮��
	double _ratio = (_contentRatio < 0) ? 0 : _contentRatio;
	_ratio = (_ratio > 1) ? 1 : _ratio;

	//��ֵֵ���Ȼ��Ʊ�����ɫ��䣬�ٻ���������ɫ��䣬�ȴ�����Ⱦ�������϶�������¶��㣬Ȼ������ɫ
	_dstRect = { _leftUpPosition.x, _leftUpPosition.y, _size.x, _size.y };
	boxRGBA(_renderer, _dstRect.x, _dstRect.y, _dstRect.x + _dstRect.w, _dstRect.y + _dstRect.h,
		_backgroundColor.r, _backgroundColor.g, _backgroundColor.b, _backgroundColor.a);
	//��������»���Բ�Ǿ��Σ����贫��Բ�ǰ뾶
	//roundedBoxRGBA(_renderer, _dstRect.x, _dstRect.y, _dstRect.x + _dstRect.w, _dstRect.y + _dstRect.h,
	//	4, _backgroundColor.r, _backgroundColor.g, _backgroundColor.b, _backgroundColor.a);

	//�������ݵ������Σ���ˮƽ��Ȱ��ձ���ʵʱ���£��������ݱ߿�����խ��������ɫ���൱����������ɫ����Χ�γ�һȦ�߿�
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
	//����Բ���
	filledCircleRGBA(_renderer, _leftUpPosition.x, _leftUpPosition.y, (Sint16)_radius,
		_contentColor.r, _contentColor.g, _contentColor.b, _contentColor.a);
	//����Բ�߿�
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