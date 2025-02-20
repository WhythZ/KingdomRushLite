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
	//获取字体
	static TTF_Font* _font = ResourceManager::Instance()->GetFontPool().find(FontResID::Pixel_CN)->second;

	#pragma region Clear
	//由于每帧OnUpdate函数都会生成一张文本的纹理，故先清除掉上一帧的遗留垃圾
	SDL_DestroyTexture(coinTextTexture);
	coinTextTexture = nullptr;
	#pragma endregion

	#pragma region CoinText
	//拿到金币数量，强转为整形后转化为字符串
	std::string _coinNumStr = std::to_string((int)ProcessManager::Instance()->GetCurrentCoinNum());
	//先将文本以特定字体加载到内存中
	SDL_Surface* _coinTextSurface = TTF_RenderText_Blended(_font, _coinNumStr.c_str(), coinTextColor);
	//获取转化后的图片的长宽
	coinTextWidth = _coinTextSurface->w; coinTextHeight = _coinTextSurface->h;
	//然后再将其转化为纹理格式
	coinTextTexture = SDL_CreateTextureFromSurface(_renderer, _coinTextSurface);
	//然后清理已经无用了的Surface垃圾
	SDL_FreeSurface(_coinTextSurface);
	#pragma endregion
}

void StatusUI::OnRender(SDL_Renderer* _renderer)
{
	//存储最终渲染完成的目标状态栏UI矩形图
	static SDL_Rect _dstRect;

	#pragma region GetLoadedAssets
	static const TexturePool& _texPool = ResourceManager::Instance()->GetTexturePool();
	//获取各图标的纹理
	static SDL_Texture* coinIconTex = _texPool.find(TextureResID::UI_Coin)->second;
	static SDL_Texture* heartIconTex = _texPool.find(TextureResID::UI_Heart)->second;
	static SDL_Texture* homeIconTex = _texPool.find(TextureResID::UI_Avatar_Home)->second;
	static SDL_Texture* playerIconTex = _texPool.find(TextureResID::UI_Avatar_Player)->second;
	#pragma endregion
}