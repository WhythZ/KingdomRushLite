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
	//�Ƚ��ı����ض�������ص��ڴ���
	static TTF_Font* _font = ResourceManager::Instance()->GetFontPool().find(FontResID::Ipix)->second;
	SDL_Surface* _winLoseTextSurface = TTF_RenderText_Blended(_font, _winLoseStr.c_str(), winLoseTextColor);
	//��ȡת�����ͼƬ�ĳ���
	winLoseTextSize = { _winLoseTextSurface->w, _winLoseTextSurface->h };
	//Ȼ���ٽ���ת��Ϊ�����ʽ
	winLoseTextTexture = SDL_CreateTextureFromSurface(_renderer, _winLoseTextSurface);
	//Ȼ�������Ѿ������˵�Surface����
	SDL_FreeSurface(_winLoseTextSurface);
	#pragma endregion

	#pragma region WaveNumText
	static WaveManager* _wm = WaveManager::Instance();
	size_t _totalWaveNum = _wm->GetTotalWaveNum();
	size_t _currentWaveNum = _wm->GetCurrentWaveIdx() + 1;
	_currentWaveNum = (_currentWaveNum > _totalWaveNum) ? _totalWaveNum : _currentWaveNum;
	std::string _waveNumStr = "AT " + std::to_string(_currentWaveNum) + " / " + std::to_string(_totalWaveNum) + " WAVE";
	//�Ƚ��ı����ض�������ص��ڴ���
	SDL_Surface* _waveNumTextSurface = TTF_RenderText_Blended(_font, _waveNumStr.c_str(), waveNumTextColor);
	//��ȡת�����ͼƬ�ĳ���
	waveNumTextSize = { _waveNumTextSurface->w, _waveNumTextSurface->h };
	//Ȼ���ٽ���ת��Ϊ�����ʽ
	waveNumTextTexture = SDL_CreateTextureFromSurface(_renderer, _waveNumTextSurface);
	//Ȼ�������Ѿ������˵�Surface����
	SDL_FreeSurface(_waveNumTextSurface);
	#pragma endregion
}

void GameOverUI::OnRender(SDL_Renderer* _renderer)
{
	//����������Ⱦ��ط���
	static UIManager* _ui = UIManager::Instance();

	//���õ����϶�������
	static SDL_Point _positionLeftUp = { 0,0 };

	//��ȡ��Ļ���
	static const SDL_Point _windowSize = { ConfigManager::Instance()->basicPrefab.windowWidth,
		ConfigManager::Instance()->basicPrefab.windowHeight };

	#pragma region Background
	//����һ������ȫ�������ֱ���
	_ui->DrawFilledBox(_renderer, { 0,0 }, _windowSize, backgroundColor);
	#pragma endregion

	#pragma region WinLoseText
	//���л���
	_positionLeftUp.x = 0 + _windowSize.x / 2 - winLoseTextSize.x / 2;
	_positionLeftUp.y = 0 + _windowSize.y / 2 - winLoseTextSize.y / 2;
	_ui->DrawTexture(_renderer, winLoseTextTexture, _positionLeftUp, winLoseTextSize);
	#pragma endregion

	#pragma region WinLoseText
	//����ƫ�»���
	_positionLeftUp.x = 0 + _windowSize.x / 2 - waveNumTextSize.x / 2;
	_positionLeftUp.y = 0 + _windowSize.y / 2 - waveNumTextSize.y / 2 + winLoseTextSize.y + textRowDistance;
	_ui->DrawTexture(_renderer, waveNumTextTexture, _positionLeftUp, waveNumTextSize);
	#pragma endregion
}