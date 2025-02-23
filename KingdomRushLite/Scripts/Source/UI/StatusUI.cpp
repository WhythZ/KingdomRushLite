#include "../../Header/UI/StatusUI.h"
#include <string>
#include <SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>
#include "../../Header/Manager/Concrete/ResourceManager.h"
#include "../../Header/Manager/Concrete/ProcessManager.h"

void StatusUI::SetPosition(SDL_Point _position)
{
	position = _position;
}

void StatusUI::OnUpdate(SDL_Renderer* _renderer)
{
	#pragma region Clear
	//����ÿ֡OnUpdate������������һ���ı������������������һ֡����������
	SDL_DestroyTexture(coinNumTextTexture);
	coinNumTextTexture = nullptr;
	#pragma endregion

	#pragma region CoinText
	//�õ����������ǿתΪ���κ�ת��Ϊ�ַ���
	std::string _coinNumStr = std::to_string((int)ProcessManager::Instance()->GetCurrentCoinNum());
	//�Ƚ��ı����ض�������ص��ڴ���
	static TTF_Font* _font = ResourceManager::Instance()->GetFontPool().find(FontResID::Pixel_CN)->second;
	SDL_Surface* _coinTextSurface = TTF_RenderText_Blended(_font, _coinNumStr.c_str(), coinTextColor);
	//��ȡת�����ͼƬ�ĳ���
	coinNumTextSize = { _coinTextSurface->w, _coinTextSurface->h };
	//Ȼ���ٽ���ת��Ϊ�����ʽ
	coinNumTextTexture = SDL_CreateTextureFromSurface(_renderer, _coinTextSurface);
	//Ȼ�������Ѿ������˵�Surface����
	SDL_FreeSurface(_coinTextSurface);
	#pragma endregion
}

void StatusUI::OnRender(SDL_Renderer* _renderer)
{
	//��ʱ�洢ÿ����ȾԪ�أ�ÿ����Ⱦ����Ԫ��ʱ����
	static SDL_Rect _dstRect;

	#pragma region GetLoadedAssets
	static const TexturePool& _texPool = ResourceManager::Instance()->GetTexturePool();
	//��ȡ��ͼ�������
	static SDL_Texture* _homeIconTex = _texPool.find(TextureResID::UI_Avatar_Home)->second;
	static SDL_Texture* _playerIconTex = _texPool.find(TextureResID::UI_Avatar_Player)->second;
	static SDL_Texture* _healthIconTex = _texPool.find(TextureResID::UI_Heart)->second;
	static SDL_Texture* _coinIconTex = _texPool.find(TextureResID::UI_Coin)->second;

	//����ʵ��ͼ��ͼƬ�ĳߴ��ȡͼ��ͼƬ���
	static SDL_Point _homeIconSize = { 78,78 };
	static SDL_Point _playerIconSize = { 65,65 };
	static SDL_Point _healthIconSize = { 32,32 };
	static SDL_Point _coinIconSize = { 32,32 };
	//ʹ��Ѫ��ͼ���ϱ����ͼ���ϱ�ƽ�С����ͼ���±ߺͼ�ͼ���±�ƽ�еĶ��߼��
	static int _healthCoinIconDistance = _homeIconSize.y - _healthIconSize.y - _coinIconSize.y;
	#pragma endregion

	#pragma region HomeIcon
	//�ҵ�ͼ��λ��״̬�������Ͻǣ���ֱ��ʹ��״̬�������ϽǶ������궨λ
	_dstRect.x = position.x;
	_dstRect.y = position.y;
	//�����Ӧͼ��Ŀ�ߺ�ֱ����Ⱦ
	_dstRect.w = _homeIconSize.x;
	_dstRect.h = _homeIconSize.y;
	SDL_RenderCopy(_renderer, _homeIconTex, nullptr, &_dstRect);
	#pragma endregion

	#pragma region MultipleHealthIcon
	//�ж��ٵ�Ѫ�����ڼ�ͼ���Ҳ���Ⱦһ�Ŷ��ٸ�Ѫ��ͼ��
	for (int _i = 0; _i < ProcessManager::Instance()->GetCurrentHealth(); _i++)
	{
		//�Ӽҵ�ͼ���Ҳ࿪ʼ����һ�������Ⱦ����ֵ������ͼ��
		_dstRect.x = position.x + _homeIconSize.x + STATUSUI_HOME_RIGHT_DISTANCE
			+ _i * (STATUSUI_HEALTH_ICON_BETWEEN_DISTANCE + _healthIconSize.x);
		_dstRect.y = position.y;
		_dstRect.w = _healthIconSize.x;
		_dstRect.h = _healthIconSize.y;
		SDL_RenderCopy(_renderer, _healthIconTex, nullptr, &_dstRect);
	}
	#pragma endregion

	#pragma region CoinIcon
	//���ͼ�����ڼ�ͼ���ҲࡢѪ��ͼ���·�һ������
	_dstRect.x = position.x + _homeIconSize.x + STATUSUI_HOME_RIGHT_DISTANCE;
	_dstRect.y = position.y + _healthIconSize.y + _healthCoinIconDistance;
	_dstRect.w = _coinIconSize.x;
	_dstRect.h = _coinIconSize.y;
	SDL_RenderCopy(_renderer, _coinIconTex, nullptr, &_dstRect);
	#pragma endregion

	#pragma region CoinNumText
	//���������ʵʱ�����ı���λ���ڽ��ͼ���Ҳ�һ������
	_dstRect.x = (position.x + _homeIconSize.x + STATUSUI_HOME_RIGHT_DISTANCE)
		+ _coinIconSize.x + STATUSUI_COIN_ICON_NUMTEXT_DISTANCE;
	_dstRect.y = position.y + _healthIconSize.y + _healthCoinIconDistance;
	_dstRect.w = coinNumTextSize.x;
	_dstRect.h = coinNumTextSize.y;
	SDL_RenderCopy(_renderer, coinNumTextTexture, nullptr, &_dstRect);
	#pragma endregion

	#pragma region PlayerIcon
	//���ͷ��ͼ��ĳߴ�ȼ�ͷ��ͼ����С��Ҫ�ö����е�����ֱ�϶���
	_dstRect.x = position.x + (_homeIconSize.x - _playerIconSize.x) / 2;
	_dstRect.y = position.y + _homeIconSize.y + STATUSUI_HOME_PLAYER_ICON_DISTANCE;
	_dstRect.w = _playerIconSize.x;
	_dstRect.h = _playerIconSize.y;
	SDL_RenderCopy(_renderer, _playerIconTex, nullptr, &_dstRect);
	#pragma endregion

	#pragma region PlayerMpBar
	//��ҷ���ֵ�������϶�����Ѫ��ͼ��/���ͼ������϶�������ֱ�϶��룬�Ȼ��Ʒ���ֵ�����ݵı�����䣬��״ΪԲ�Ǿ���
	_dstRect.x = position.x + _homeIconSize.x + STATUSUI_HOME_RIGHT_DISTANCE;
	_dstRect.y = position.y + _homeIconSize.y + STATUSUI_HOME_PLAYER_ICON_DISTANCE;
	_dstRect.w = mpBarSize.x;
	_dstRect.h = mpBarSize.y;
	//��������Բ�Ǿ��Σ��ȴ�����Ⱦ�������϶�������¶��㣬Ȼ����Բ�ǰ뾶���������ɫ
	roundedBoxRGBA(_renderer, _dstRect.x, _dstRect.y, _dstRect.x + _dstRect.w, _dstRect.y + _dstRect.h,
		4, mpBarContentBackColor.r, mpBarContentBackColor.g, mpBarContentBackColor.b, mpBarContentBackColor.a);

	//�����Ʒ���ֵ�������ݣ���ˮƽ���ʵʱ���£��������ݱ߿�����խ��������ɫ���൱����������ɫ����Χ�γ�һȦ�߿�
	_dstRect.x = (position.x + _homeIconSize.x + STATUSUI_HOME_RIGHT_DISTANCE) + mpBarBorderThickness;
	_dstRect.y = (position.y + _homeIconSize.y + STATUSUI_HOME_PLAYER_ICON_DISTANCE) + mpBarBorderThickness;
	_dstRect.w = mpBarSize.x - 2 * mpBarBorderThickness;
	_dstRect.h = mpBarSize.y - 2 * mpBarBorderThickness;
	roundedBoxRGBA(_renderer, _dstRect.x, _dstRect.y, _dstRect.x + _dstRect.w, _dstRect.y + _dstRect.h,
		4, mpBarContentForeColor.r, mpBarContentForeColor.g, mpBarContentForeColor.b, mpBarContentForeColor.a);
	#pragma endregion
}