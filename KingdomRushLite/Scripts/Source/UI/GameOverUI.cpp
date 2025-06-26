#include "../../Header/UI/GameOverUI.h"
#include <string>
#include "../../Header/Manager/Concrete/UIManager.h"
#include "../../Header/Manager/Concrete/ResourceManager.h"
#include "../../Header/Manager/Concrete/ConfigManager.h"
#include "../../Header/Manager/Concrete/ProcessManager.h"
#include "../../Header/Manager/Concrete/WaveManager.h"

void GameOverUI::OnUpdate(SDL_Renderer* _renderer)
{
	#pragma region Clear
	SDL_DestroyTexture(winLoseTextTexture);
	winLoseTextTexture = nullptr;
	SDL_DestroyTexture(waveNumTextTexture);
	waveNumTextTexture = nullptr;
	#pragma endregion

	#pragma region WinLostText
	static ProcessManager* _pm = ProcessManager::Instance();
	std::string _winLoseStr = "";
	if (_pm->isWin)
		_winLoseStr = "YOU WIN";
	else
		_winLoseStr = "YOU LOSE";
	//先将文本以特定字体加载到内存中
	static TTF_Font* _font = ResourceManager::Instance()->GetFontPool().find(FontResID::Ipix)->second;
	SDL_Surface* _winLoseTextSurface = TTF_RenderText_Blended(_font, _winLoseStr.c_str(), winLoseTextColor);
	//获取转化后的图片的长宽
	winLoseTextSize = { _winLoseTextSurface->w, _winLoseTextSurface->h };
	//然后再将其转化为纹理格式
	winLoseTextTexture = SDL_CreateTextureFromSurface(_renderer, _winLoseTextSurface);
	//然后清理已经无用了的Surface垃圾
	SDL_FreeSurface(_winLoseTextSurface);
	#pragma endregion

	#pragma region WaveNumText
	static WaveManager* _wm = WaveManager::Instance();
	size_t _totalWaveNum = _wm->GetTotalWaveNum();
	size_t _currentWaveNum = _wm->GetCurrentWaveIdx() + 1;
	_currentWaveNum = (_currentWaveNum > _totalWaveNum) ? _totalWaveNum : _currentWaveNum;
	std::string _waveNumStr = "AT " + std::to_string(_currentWaveNum) + " / " + std::to_string(_totalWaveNum) + " WAVE";
	//先将文本以特定字体加载到内存中
	SDL_Surface* _waveNumTextSurface = TTF_RenderText_Blended(_font, _waveNumStr.c_str(), waveNumTextColor);
	//获取转化后的图片的长宽
	waveNumTextSize = { _waveNumTextSurface->w, _waveNumTextSurface->h };
	//然后再将其转化为纹理格式
	waveNumTextTexture = SDL_CreateTextureFromSurface(_renderer, _waveNumTextSurface);
	//然后清理已经无用了的Surface垃圾
	SDL_FreeSurface(_waveNumTextSurface);
	#pragma endregion
}

void GameOverUI::OnRender(SDL_Renderer* _renderer)
{
	//引入纹理渲染相关方法
	static UIManager* _ui = UIManager::Instance();

	//复用的左上顶点坐标
	static SDL_Point _positionLeftUp = { 0,0 };

	//获取屏幕宽高
	static const SDL_Point _windowSize = { ConfigManager::Instance()->basicPrefab.windowWidth,
		ConfigManager::Instance()->basicPrefab.windowHeight };

	#pragma region Background
	//绘制一个覆盖全屏的遮罩背景
	_ui->DrawFilledBox(_renderer, { 0,0 }, _windowSize, backgroundColor);
	#pragma endregion

	#pragma region WinLoseText
	//居中绘制
	_positionLeftUp.x = 0 + _windowSize.x / 2 - winLoseTextSize.x / 2;
	_positionLeftUp.y = 0 + _windowSize.y / 2 - winLoseTextSize.y / 2;
	_ui->DrawTexture(_renderer, winLoseTextTexture, _positionLeftUp, winLoseTextSize);
	#pragma endregion

	#pragma region WinLoseText
	//居中偏下绘制
	_positionLeftUp.x = 0 + _windowSize.x / 2 - waveNumTextSize.x / 2;
	_positionLeftUp.y = 0 + _windowSize.y / 2 - waveNumTextSize.y / 2 + winLoseTextSize.y + textRowDistance;
	_ui->DrawTexture(_renderer, waveNumTextTexture, _positionLeftUp, waveNumTextSize);
	#pragma endregion
}