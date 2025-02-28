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
	//����ÿ֡OnUpdate������������һ���ı������������������һ֡����������
	SDL_DestroyTexture(coinNumTextTexture);
	coinNumTextTexture = nullptr;
	SDL_DestroyTexture(waveNumTextTexture);
	waveNumTextTexture = nullptr;
	#pragma endregion

	#pragma region CoinNumText
	//�õ����������ǿתΪ���κ�ת��Ϊ�ַ���
	static ProcessManager* _pm = ProcessManager::Instance();
	std::string _coinNumStr = std::to_string((int)_pm->GetCurrentCoinNum());
	//�Ƚ��ı����ض�������ص��ڴ���
	static TTF_Font* _font = ResourceManager::Instance()->GetFontPool().find(FontResID::Ipix)->second;
	SDL_Surface* _coinTextSurface = TTF_RenderText_Blended(_font, _coinNumStr.c_str(), coinNumTextColor);
	//��ȡת�����ͼƬ�ĳ���
	coinNumTextSize = { _coinTextSurface->w, _coinTextSurface->h };
	//Ȼ���ٽ���ת��Ϊ�����ʽ
	coinNumTextTexture = SDL_CreateTextureFromSurface(_renderer, _coinTextSurface);
	//Ȼ�������Ѿ������˵�Surface����
	SDL_FreeSurface(_coinTextSurface);
	#pragma endregion

	#pragma region WaveNumText
	static WaveManager* _wm = WaveManager::Instance();
	std::string _waveNumStr = "WAVE " + std::to_string(_wm->GetCurrentWaveIdx() + 1);
	//�Ƚ��ı����ض�������ص��ڴ���
	SDL_Surface* _waveNumTextSurface = TTF_RenderText_Blended(_font, _waveNumStr.c_str(), waveNumTextColor);
	//��ȡת�����ͼƬ�ĳ���
	waveNumTextSize = { _waveNumTextSurface->w, _waveNumTextSurface->h };
	//Ȼ���ٽ���ת��Ϊ�����ʽ
	waveNumTextTexture = SDL_CreateTextureFromSurface(_renderer, _waveNumTextSurface);
	//Ȼ�������Ѿ������˵�Surface����
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
	//����������Ⱦ��ط���
	static UIManager* _ui = UIManager::Instance();

	//���õ����϶�������
	static SDL_Point _positionLeftUp = { 0,0 };

	//��ȡ��Ļ���
	static const SDL_Point _windowSize = { ConfigManager::Instance()->basicPrefab.windowWidth,
		ConfigManager::Instance()->basicPrefab.windowHeight };

	#pragma region MultipleHealthIcon
	//�ж��ٵ�Ѫ������Ⱦһ�Ŷ��ٸ�Ѫ��ͼ��
	for (int _i = 0; _i < ProcessManager::Instance()->GetCurrentHealth(); _i++)
	{
		_positionLeftUp.x = 0 + _i * (healthHeartIconBetweenDistance + healthHeartIconSize.x);
		_positionLeftUp.y = 0;
		_ui->DrawTexture(_renderer, TextureResID::UI_Icon_HealthHeart, _positionLeftUp, healthHeartIconSize);
	}
	#pragma endregion

	#pragma region CoinIcon
	//���ͼ������Ѫ��ͼ���·�һ������
	_positionLeftUp.x = 0;
	_positionLeftUp.y = 0 + healthHeartIconSize.y + rowBetweenDistance;
	_ui->DrawTexture(_renderer, TextureResID::UI_Icon_Coin, _positionLeftUp, coinIconSize);
	#pragma endregion

	#pragma region CoinNumText
	//���������ʵʱ�����ı���λ���ڽ��ͼ���Ҳ�һ������
	_positionLeftUp.x = 0 + coinIconSize.x + iconTextBetweenDistance;
	_positionLeftUp.y = 0 + healthHeartIconSize.y + rowBetweenDistance;
	_ui->DrawTexture(_renderer, coinNumTextTexture, _positionLeftUp, coinNumTextSize);
	#pragma endregion

	#pragma region WaveNumText
	//��Ⱦ����Ļ���Ͻ�
	_positionLeftUp.x = 0 + _windowSize.x - waveNumTextSize.x;
	_positionLeftUp.y = 0;
	_ui->DrawTexture(_renderer, waveNumTextTexture, _positionLeftUp, waveNumTextSize);
	#pragma endregion

	#pragma region PlayerIcon
	//�����ͼ����Ⱦ����Ļ���½�
	_positionLeftUp.x = 0;
	_positionLeftUp.y = 0 + _windowSize.y - playerIconSize.y;
	//��ȡ��ǰ������࣬���ƶ�Ӧͼ��
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
	//���Ʒ���ֵ�������ͷ��ͼ���Ҳ࣬�ֱ������������
	_positionLeftUp.x = 0 + playerIconSize.x + iconBarBetweenDistance;
	_positionLeftUp.y = 0 + _windowSize.y - playerIconSize.y;
	_ui->DrawDynamicBar(_renderer, _positionLeftUp, mpBarSize, mpBarBorderThickness,
		mpBarBackgroundColor, mpBarContentColor, skill00MpBarRatio);

	//�ڶ�������ֵ���ڵ�һ�����·�
	_positionLeftUp.x = 0 + playerIconSize.x + iconBarBetweenDistance;
	_positionLeftUp.y = (0 + _windowSize.y - playerIconSize.y) + mpBarSize.y + rowBetweenDistance;
	_ui->DrawDynamicBar(_renderer, _positionLeftUp, mpBarSize, mpBarBorderThickness,
		mpBarBackgroundColor, mpBarContentColor, skill01MpBarRatio);
	#pragma endregion
}