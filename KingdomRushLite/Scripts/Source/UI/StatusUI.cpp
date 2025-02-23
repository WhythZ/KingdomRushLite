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
	//由于每帧OnUpdate函数都会生成一张文本的纹理，故先清除掉上一帧的遗留垃圾
	SDL_DestroyTexture(coinNumTextTexture);
	coinNumTextTexture = nullptr;
	#pragma endregion

	#pragma region CoinText
	//拿到金币数量，强转为整形后转化为字符串
	std::string _coinNumStr = std::to_string((int)ProcessManager::Instance()->GetCurrentCoinNum());
	//先将文本以特定字体加载到内存中
	static TTF_Font* _font = ResourceManager::Instance()->GetFontPool().find(FontResID::Pixel_CN)->second;
	SDL_Surface* _coinTextSurface = TTF_RenderText_Blended(_font, _coinNumStr.c_str(), coinTextColor);
	//获取转化后的图片的长宽
	coinNumTextSize = { _coinTextSurface->w, _coinTextSurface->h };
	//然后再将其转化为纹理格式
	coinNumTextTexture = SDL_CreateTextureFromSurface(_renderer, _coinTextSurface);
	//然后清理已经无用了的Surface垃圾
	SDL_FreeSurface(_coinTextSurface);
	#pragma endregion
}

void StatusUI::OnRender(SDL_Renderer* _renderer)
{
	//临时存储每个渲染元素，每次渲染单个元素时复用
	static SDL_Rect _dstRect;

	#pragma region GetLoadedAssets
	static const TexturePool& _texPool = ResourceManager::Instance()->GetTexturePool();
	//获取各图标的纹理
	static SDL_Texture* _homeIconTex = _texPool.find(TextureResID::UI_Avatar_Home)->second;
	static SDL_Texture* _playerIconTex = _texPool.find(TextureResID::UI_Avatar_Player)->second;
	static SDL_Texture* _healthIconTex = _texPool.find(TextureResID::UI_Heart)->second;
	static SDL_Texture* _coinIconTex = _texPool.find(TextureResID::UI_Coin)->second;

	//依照实际图标图片的尺寸获取图标图片宽高
	static SDL_Point _homeIconSize = { 78,78 };
	static SDL_Point _playerIconSize = { 65,65 };
	static SDL_Point _healthIconSize = { 32,32 };
	static SDL_Point _coinIconSize = { 32,32 };
	//使得血量图标上边与家图标上边平行、金币图标下边和家图标下边平行的二者间距
	static int _healthCoinIconDistance = _homeIconSize.y - _healthIconSize.y - _coinIconSize.y;
	#pragma endregion

	#pragma region HomeIcon
	//家的图标位于状态栏的左上角，故直接使用状态栏的左上角顶点坐标定位
	_dstRect.x = position.x;
	_dstRect.y = position.y;
	//赋予对应图标的宽高后，直接渲染
	_dstRect.w = _homeIconSize.x;
	_dstRect.h = _homeIconSize.y;
	SDL_RenderCopy(_renderer, _homeIconTex, nullptr, &_dstRect);
	#pragma endregion

	#pragma region MultipleHealthIcon
	//有多少点血量就在家图标右侧渲染一排多少个血量图标
	for (int _i = 0; _i < ProcessManager::Instance()->GetCurrentHealth(); _i++)
	{
		//从家的图标右侧开始，以一定间距渲染生命值数量个图标
		_dstRect.x = position.x + _homeIconSize.x + STATUSUI_HOME_RIGHT_DISTANCE
			+ _i * (STATUSUI_HEALTH_ICON_BETWEEN_DISTANCE + _healthIconSize.x);
		_dstRect.y = position.y;
		_dstRect.w = _healthIconSize.x;
		_dstRect.h = _healthIconSize.y;
		SDL_RenderCopy(_renderer, _healthIconTex, nullptr, &_dstRect);
	}
	#pragma endregion

	#pragma region CoinIcon
	//金币图标置于家图标右侧、血量图标下方一定距离
	_dstRect.x = position.x + _homeIconSize.x + STATUSUI_HOME_RIGHT_DISTANCE;
	_dstRect.y = position.y + _healthIconSize.y + _healthCoinIconDistance;
	_dstRect.w = _coinIconSize.x;
	_dstRect.h = _coinIconSize.y;
	SDL_RenderCopy(_renderer, _coinIconTex, nullptr, &_dstRect);
	#pragma endregion

	#pragma region CoinNumText
	//金币数量的实时更新文本，位置在金币图标右侧一定距离
	_dstRect.x = (position.x + _homeIconSize.x + STATUSUI_HOME_RIGHT_DISTANCE)
		+ _coinIconSize.x + STATUSUI_COIN_ICON_NUMTEXT_DISTANCE;
	_dstRect.y = position.y + _healthIconSize.y + _healthCoinIconDistance;
	_dstRect.w = coinNumTextSize.x;
	_dstRect.h = coinNumTextSize.y;
	SDL_RenderCopy(_renderer, coinNumTextTexture, nullptr, &_dstRect);
	#pragma endregion

	#pragma region PlayerIcon
	//玩家头像图标的尺寸比家头像图标略小，要让二者中点在竖直上对齐
	_dstRect.x = position.x + (_homeIconSize.x - _playerIconSize.x) / 2;
	_dstRect.y = position.y + _homeIconSize.y + STATUSUI_HOME_PLAYER_ICON_DISTANCE;
	_dstRect.w = _playerIconSize.x;
	_dstRect.h = _playerIconSize.y;
	SDL_RenderCopy(_renderer, _playerIconTex, nullptr, &_dstRect);
	#pragma endregion

	#pragma region PlayerMpBar
	//玩家法力值条的左上顶点与血量图标/金币图标的左上顶点在竖直上对齐，先绘制法力值条内容的背景填充，形状为圆角矩形
	_dstRect.x = position.x + _homeIconSize.x + STATUSUI_HOME_RIGHT_DISTANCE;
	_dstRect.y = position.y + _homeIconSize.y + STATUSUI_HOME_PLAYER_ICON_DISTANCE;
	_dstRect.w = mpBarSize.x;
	_dstRect.h = mpBarSize.y;
	//绘制填充的圆角矩形，先传入渲染器与左上顶点和右下顶点，然后是圆角半径，最后是颜色
	roundedBoxRGBA(_renderer, _dstRect.x, _dstRect.y, _dstRect.x + _dstRect.w, _dstRect.y + _dstRect.h,
		4, mpBarContentBackColor.r, mpBarContentBackColor.g, mpBarContentBackColor.b, mpBarContentBackColor.a);

	//最后绘制法力值条的内容（其水平宽度实时更新），其依据边框宽度收窄，背景颜色就相当于在内容颜色的外围形成一圈边框
	_dstRect.x = (position.x + _homeIconSize.x + STATUSUI_HOME_RIGHT_DISTANCE) + mpBarBorderThickness;
	_dstRect.y = (position.y + _homeIconSize.y + STATUSUI_HOME_PLAYER_ICON_DISTANCE) + mpBarBorderThickness;
	_dstRect.w = mpBarSize.x - 2 * mpBarBorderThickness;
	_dstRect.h = mpBarSize.y - 2 * mpBarBorderThickness;
	roundedBoxRGBA(_renderer, _dstRect.x, _dstRect.y, _dstRect.x + _dstRect.w, _dstRect.y + _dstRect.h,
		4, mpBarContentForeColor.r, mpBarContentForeColor.g, mpBarContentForeColor.b, mpBarContentForeColor.a);
	#pragma endregion
}