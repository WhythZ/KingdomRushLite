#include "../../../Header/Bullet/Concrete/Axe.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header//Manager/Concrete/AudioManager.h"

Axe::Axe()
{
	//设置子弹类型
	type = BulletType::Axe;

	//根据每帧动画的图片文件实际尺寸设置
	size.x = 48; size.y = 48;

	#pragma region SetAnimation
	//获取图片资源并设置动画帧
	static SDL_Texture* _animSprites = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Bullet_Axe)->second;
	static const std::vector<int> _animSpritesIdices = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
	animFlying.SetAnimFrames(_animSprites, 4, 2, _animSpritesIdices);
	animFlying.SetLoop(true);
	#pragma endregion
}

void Axe::OnCollide(Enemy* _enemy)
{
	//调用父类原方法
	Bullet::OnCollide(_enemy);

	//随机播放一种命中音效
	switch (rand() % 3)
	{
	case 0:
		AudioManager::Instance()->PlaySFX(SoundResID::Bullet_Axe_Hit_1);
		break;
	case 1:
		AudioManager::Instance()->PlaySFX(SoundResID::Bullet_Axe_Hit_2);
		break;
	case 2:
		AudioManager::Instance()->PlaySFX(SoundResID::Bullet_Axe_Hit_3);
		break;
	default:
		break;
	}

	#pragma region Effect
	//斧头子弹命中敌人的特殊效果，即给敌人减速（若干时间后自动恢复原速）
	_enemy->SlowDown();
	#pragma endregion
}