#include "../../../Header/Drop/Concrete/Coin.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header/Manager/Concrete/ProcessManager.h"
#include "../../../Header/Manager/Concrete/AudioManager.h"

//ע�⹹�캯���Ķ�̬д��
Coin::Coin() : Drop::Drop()
{
	//���óߴ�
	size.x = 16; size.y = 16;

	#pragma region SetAnimation
	static SDL_Texture* _animSprites = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Item_Coin)->second;
	static const std::vector<int> _animSpritesIdices = { 0 };
	animIdle.SetAnimFrames(_animSprites, 1, 1, _animSpritesIdices);
	animIdle.SetLoop(true);
	#pragma endregion
}

void Coin::OnCollide()
{
	//�����ҵ����������
	ProcessManager::Instance()->IncreaseCoinNumBy(coinReward);

	//ʰȡ��Ч
	AudioManager::Instance()->PlaySFX(SoundResID::Pick_Coin);

	Drop::OnCollide();
}