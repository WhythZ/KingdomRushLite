#include "../../Header/UI/StatusUI.h"
#include <string>
#include <SDL_ttf.h>
#include "../../Header/Manager/Concrete/UIManager.h"
#include "../../Header/Manager/Concrete/ResourceManager.h"
#include "../../Header/Manager/Concrete/ConfigManager.h"
#include "../../Header/Manager/Concrete/ProcessManager.h"
#include "../../Header/Manager/Concrete/WaveManager.h"
#include "../../Header/Manager/Concrete/PlayerManager.h"

void StatusUI::OnUpdate(SDL_Renderer* _renderer)
{
	#pragma region Clear
	//由于每帧OnUpdate函数都会生成一张文本的纹理，故先清除掉上一帧的遗留垃圾
	SDL_DestroyTexture(coinNumTextTexture);
	coinNumTextTexture = nullptr;
	SDL_DestroyTexture(waveNumTextTexture);
	waveNumTextTexture = nullptr;
	#pragma endregion

	#pragma region CoinNumText
	//拿到金币数量，强转为整形后转化为字符串
	static ProcessManager* _pm = ProcessManager::Instance();
	std::string _coinNumStr = std::to_string((int)_pm->GetCurrentCoinNum());
	//先将文本以特定字体加载到内存中
	static TTF_Font* _font = ResourceManager::Instance()->GetFontPool().find(FontResID::Ipix)->second;
	SDL_Surface* _coinTextSurface = TTF_RenderText_Blended(_font, _coinNumStr.c_str(), coinNumTextColor);
	//获取转化后的图片的长宽
	coinNumTextSize = { _coinTextSurface->w, _coinTextSurface->h };
	//然后再将其转化为纹理格式
	coinNumTextTexture = SDL_CreateTextureFromSurface(_renderer, _coinTextSurface);
	//然后清理已经无用了的Surface垃圾
	SDL_FreeSurface(_coinTextSurface);
	#pragma endregion

	#pragma region WaveNumText
	static WaveManager* _wm = WaveManager::Instance();
	std::string _waveNumStr = "WAVE " + std::to_string(_wm->GetCurrentWaveIdx() + 1);
	//先将文本以特定字体加载到内存中
	SDL_Surface* _waveNumTextSurface = TTF_RenderText_Blended(_font, _waveNumStr.c_str(), waveNumTextColor);
	//获取转化后的图片的长宽
	waveNumTextSize = { _waveNumTextSurface->w, _waveNumTextSurface->h };
	//然后再将其转化为纹理格式
	waveNumTextTexture = SDL_CreateTextureFromSurface(_renderer, _waveNumTextSurface);
	//然后清理已经无用了的Surface垃圾
	SDL_FreeSurface(_waveNumTextSurface);
	#pragma endregion

	#pragma region MpBarRatio
	static Player* _player = PlayerManager::Instance()->player;
	skill00MpBarRatio = _player->GetSkill00CooldownRatio();
	skill01MpBarRatio = _player->GetSkill01CooldownRatio();
	#pragma endregion
}

void StatusUI::OnRender(SDL_Renderer* _renderer)
{
	//引入纹理渲染相关方法
	static UIManager* _ui = UIManager::Instance();

	//复用的左上顶点坐标
	static SDL_Point _positionLeftUp = { 0,0 };

	//获取屏幕宽高
	static const SDL_Point _windowSize = { ConfigManager::Instance()->basicPrefab.windowWidth,
		ConfigManager::Instance()->basicPrefab.windowHeight };

	#pragma region MultipleHealthIcon
	//有多少点血量就渲染一排多少个血量图标
	for (int _i = 0; _i < ProcessManager::Instance()->GetCurrentHealth(); _i++)
	{
		_positionLeftUp.x = 0 + _i * (healthHeartIconBetweenDistance + healthHeartIconSize.x);
		_positionLeftUp.y = 0;
		_ui->DrawTexture(_renderer, TextureResID::UI_Icon_HealthHeart, _positionLeftUp, healthHeartIconSize);
	}
	#pragma endregion

	#pragma region CoinIcon
	//金币图标置于血量图标下方一定距离
	_positionLeftUp.x = 0;
	_positionLeftUp.y = 0 + healthHeartIconSize.y + rowBetweenDistance;
	_ui->DrawTexture(_renderer, TextureResID::UI_Icon_Coin, _positionLeftUp, coinIconSize);
	#pragma endregion

	#pragma region CoinNumText
	//金币数量的实时更新文本，位置在金币图标右侧一定距离
	_positionLeftUp.x = 0 + coinIconSize.x + iconTextBetweenDistance;
	_positionLeftUp.y = 0 + healthHeartIconSize.y + rowBetweenDistance;
	_ui->DrawTexture(_renderer, coinNumTextTexture, _positionLeftUp, coinNumTextSize);
	#pragma endregion

	#pragma region WaveNumText
	//渲染在屏幕右上角
	_positionLeftUp.x = 0 + _windowSize.x - waveNumTextSize.x;
	_positionLeftUp.y = 0;
	_ui->DrawTexture(_renderer, waveNumTextTexture, _positionLeftUp, waveNumTextSize);
	#pragma endregion

	#pragma region PlayerIcon
	//将玩家图标渲染在屏幕左下角
	_positionLeftUp.x = 0;
	_positionLeftUp.y = 0 + _windowSize.y - playerIconSize.y;
	//获取当前玩家种类，绘制对应图标
	PlayerType _playerType = PlayerManager::Instance()->player->GetType();
	TextureResID _playerIconTextureID;
	switch (_playerType)
	{
	case PlayerType::Dragon:
		_playerIconTextureID = TextureResID::UI_Icon_Player_Dragon;
		break;
	default:
		//_playerIconTextureID = TextureResID::;
		break;
	}
	_ui->DrawTexture(_renderer, _playerIconTextureID, _positionLeftUp, playerIconSize);
	#pragma endregion

	#pragma region PlayerMpBar
	//绘制法力值条在玩家头像图标右侧，分别绘制两个技能
	_positionLeftUp.x = 0 + playerIconSize.x + iconBarBetweenDistance;
	_positionLeftUp.y = 0 + _windowSize.y - playerIconSize.y;
	_ui->DrawDynamicBar(_renderer, _positionLeftUp, mpBarSize, mpBarBorderThickness,
		mpBarBackgroundColor, mpBarContentColor, skill00MpBarRatio);

	//第二个法力值条在第一个的下方
	_positionLeftUp.x = 0 + playerIconSize.x + iconBarBetweenDistance;
	_positionLeftUp.y = (0 + _windowSize.y - playerIconSize.y) + mpBarSize.y + rowBetweenDistance;
	_ui->DrawDynamicBar(_renderer, _positionLeftUp, mpBarSize, mpBarBorderThickness,
		mpBarBackgroundColor, mpBarContentColor, skill01MpBarRatio);
	#pragma endregion
}