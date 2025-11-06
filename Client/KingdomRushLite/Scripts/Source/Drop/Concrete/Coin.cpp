#include "../../../Header/Drop/Concrete/Coin.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header/Manager/Concrete/ProcessManager.h"
#include "../../../Header/Manager/Concrete/AudioManager.h"

//注意构造函数的多态写法
Coin::Coin() : Drop::Drop()
{
	//设置尺寸
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
	//给予金币递增后再清除
	ProcessManager::Instance()->IncreaseCoinNumBy(coinReward);

	//拾取音效
	AudioManager::Instance()->PlaySFX(SoundResID::Pick_Coin);

	Drop::OnCollide();
}