#include "../../../Header/Bullet/Concrete/Axe.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header//Manager/Concrete/AudioManager.h"

Axe::Axe()
{
	//�����ӵ�����
	type = BulletType::Axe;

	//����ÿ֡������ͼƬ�ļ�ʵ�ʳߴ�����
	size.x = 48; size.y = 48;

	#pragma region SetAnimation
	//��ȡͼƬ��Դ�����ö���֡
	static SDL_Texture* _animSprites = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Bullet_Axe)->second;
	static const std::vector<int> _animSpritesIdices = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
	animFlying.SetAnimFrames(_animSprites, 4, 2, _animSpritesIdices);
	animFlying.SetLoop(true);
	#pragma endregion
}

void Axe::OnCollide(Enemy* _enemy)
{
	//���ø���ԭ����
	Bullet::OnCollide(_enemy);

	//�������һ��������Ч
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
	//��ͷ�ӵ����е��˵�����Ч�����������˼��٣�����ʱ����Զ��ָ�ԭ�٣�
	_enemy->SlowDown();
	#pragma endregion
}