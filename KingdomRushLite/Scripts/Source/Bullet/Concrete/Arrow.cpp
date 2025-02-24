#include "../../../Header/Bullet/Concrete/Arrow.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header//Manager/Concrete/AudioManager.h"

Arrow::Arrow()
{
	//�����ӵ�����
	type = BulletType::Arrow;

	//��ȡͼƬ��Դ�����ö���֡
	static SDL_Texture* _animSprites = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Bullet_Arrow)->second;
	static const std::vector<int> _animSpritesIdices = { 0, 1 };
	animFlying.SetAnimFrames(_animSprites, 2, 1, _animSpritesIdices);
	animFlying.SetLoop(true);

	//���Ķ�������Ҫ�г����
	canRotate = true;

	//����ÿ֡������ͼƬ�ļ�ʵ�ʳߴ�����
	size.x = 48; size.y = 48;
}

void Arrow::OnCollide(Enemy* _enemy)
{
	//���ø���ԭ����
	Bullet::OnCollide(_enemy);

	//�������һ��������Ч
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