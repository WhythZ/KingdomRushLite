#include "../../Header/UI/StatusUI.h"
#include <string>
#include <SDL_ttf.h>
#include "../../Header/Manager/Concrete/ResourceManager.h"
#include "../../Header/Manager/Concrete/ProcessManager.h"

void StatusUI::SetPosition(SDL_Point _position)
{
	position = _position;
}

void StatusUI::OnUpdate(double _delta, SDL_Renderer* _renderer)
{
	//��ȡ����
	static TTF_Font* _font = ResourceManager::Instance()->GetFontPool().find(FontResID::Pixel_CN)->second;

	#pragma region Clear
	//����ÿ֡OnUpdate������������һ���ı������������������һ֡����������
	SDL_DestroyTexture(coinTextTexture);
	coinTextTexture = nullptr;
	#pragma endregion

	#pragma region CoinText
	//�õ����������ǿתΪ���κ�ת��Ϊ�ַ���
	std::string _coinNumStr = std::to_string((int)ProcessManager::Instance()->GetCurrentCoinNum());
	//�Ƚ��ı����ض�������ص��ڴ���
	SDL_Surface* _coinTextSurface = TTF_RenderText_Blended(_font, _coinNumStr.c_str(), coinTextColor);
	//��ȡת�����ͼƬ�ĳ���
	coinTextWidth = _coinTextSurface->w; coinTextHeight = _coinTextSurface->h;
	//Ȼ���ٽ���ת��Ϊ�����ʽ
	coinTextTexture = SDL_CreateTextureFromSurface(_renderer, _coinTextSurface);
	//Ȼ�������Ѿ������˵�Surface����
	SDL_FreeSurface(_coinTextSurface);
	#pragma endregion
}

void StatusUI::OnRender(SDL_Renderer* _renderer)
{
	//�洢������Ⱦ��ɵ�Ŀ��״̬��UI����ͼ
	static SDL_Rect _dstRect;

	#pragma region GetLoadedAssets
	static const TexturePool& _texPool = ResourceManager::Instance()->GetTexturePool();
	//��ȡ��ͼ�������
	static SDL_Texture* coinIconTex = _texPool.find(TextureResID::UI_Coin)->second;
	static SDL_Texture* heartIconTex = _texPool.find(TextureResID::UI_Heart)->second;
	static SDL_Texture* homeIconTex = _texPool.find(TextureResID::UI_Avatar_Home)->second;
	static SDL_Texture* playerIconTex = _texPool.find(TextureResID::UI_Avatar_Player)->second;
	#pragma endregion
}