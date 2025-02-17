#ifndef _ARROW_HPP_
#define _ARROW_HPP_

#include "../Bullet.hpp"
#include "../../Animation/Animation.hpp"
#include "../../Manager/Concrete/ResourceManager.hpp"
#include "../../Manager/Concrete/AudioManager.hpp"

class Arrow : public Bullet
{
public:
	Arrow();
	~Arrow() = default;

	void OnCollide(Enemy*) override;
};

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
		AudioManager::Instance()->PlaySFX(SoundResID::Arrow_Hit_1);
		break;
	case 1:
		AudioManager::Instance()->PlaySFX(SoundResID::Arrow_Hit_2);
		break;
	case 2:
		AudioManager::Instance()->PlaySFX(SoundResID::Arrow_Hit_3);
		break;
	default:
		break;
	}
}

#endif
