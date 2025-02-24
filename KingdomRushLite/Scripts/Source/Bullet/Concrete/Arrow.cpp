#include "../../../Header/Bullet/Concrete/Arrow.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header//Manager/Concrete/AudioManager.h"

Arrow::Arrow()
{
	//设置子弹类型
	type = BulletType::Arrow;

	//获取图片资源并设置动画帧
	static SDL_Texture* _animSprites = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Bullet_Arrow)->second;
	static const std::vector<int> _animSpritesIdices = { 0, 1 };
	animFlying.SetAnimFrames(_animSprites, 2, 1, _animSpritesIdices);
	animFlying.SetLoop(true);

	//箭的动画是需要有朝向的
	canRotate = true;

	//根据每帧动画的图片文件实际尺寸设置
	size.x = 48; size.y = 48;
}

void Arrow::OnCollide(Enemy* _enemy)
{
	//调用父类原方法
	Bullet::OnCollide(_enemy);

	//随机播放一种命中音效
	switch (rand() % 3)
	{
	case 0:
		AudioManager::Instance()->PlaySFX(SoundResID::Bullet_Arrow_Hit_1);
		break;
	case 1:
		AudioManager::Instance()->PlaySFX(SoundResID::Bullet_Arrow_Hit_2);
		break;
	case 2:
		AudioManager::Instance()->PlaySFX(SoundResID::Bullet_Arrow_Hit_3);
		break;
	default:
		break;
	}
}