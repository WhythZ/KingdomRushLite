#ifndef _SHELL_HPP_
#define _SHELL_HPP_

#include "../Bullet.hpp"
#include "../../Animation/Animation.hpp"
#include "../../Manager/Concrete/ResourceManager.hpp"
#include "../../Manager/Concrete/AudioManager.hpp"

class Shell : public Bullet
{
private:
	Animation animExploding;           //�ڵ���ը��Ķ���

public:
	Shell();
	~Shell() = default;

	void OnUpdate(double) override;
	void OnRender(SDL_Renderer*) override;
	void OnCollide(Enemy*) override;
};

Shell::Shell()
{
	//��ȡ�ڵ����е�ͼƬ��Դ�����ö���֡
	static SDL_Texture* _animFlyingSprites = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Bullet_Shell)->second;
	static const std::vector<int> _animFlyingSpritesIdices = { 0, 1 };
	animFlying.SetAnimFrames(_animFlyingSprites, 2, 1, _animFlyingSpritesIdices);
	animFlying.SetLoop(true);

	//��ȡ�ڵ���ը�Ķ���
	static SDL_Texture* _animExplodingSprites = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Bullet_Shell_Explode)->second;
	static const std::vector<int> _animExplodingSpritesIdices = { 0, 1, 3, 4, 5 };
	animExploding.SetAnimFrames(_animExplodingSprites, 5, 1, _animExplodingSpritesIdices);
	//�ڵ���������ѭ����ը���û��
	animExploding.SetLoop(false);
	//���ڵ���������ʱ����ʹ�ڵ���ʧ�Ļص�����
	animExploding.SetAnimOnFinished(
		[&]()
		{
			Invalidate();
		}
	);

	//����ÿ֡������ͼƬ�ļ�ʵ�ʳߴ�����
	size.x = 48; size.y = 48;

	//�����ڵ��ı�ը��Χ��ͬʱҲ�Ǳ�ը����֡�ĳߴ�
	damageRange = 96;
}

void Shell::OnUpdate(double _delta)
{
	//�����ǰ�ڵ�δ��ը�������ڿ���ײ��״̬����ֻ���ø��෽��
	if (CanCollide())
	{
		//�����ڵ�����ʱ�Ķ�������
		Bullet::OnUpdate(_delta);
		return;
	}

	#pragma region ExplosionAnimUpdate
	//����˵���ڵ���ǰ���ڱ�ը��״̬����ʱ����±�ը����
	animExploding.OnUpdate(_delta);
	#pragma endregion
}

void Shell::OnRender(SDL_Renderer* _renderer)
{
	//�����ǰ�ڵ�δ��ը�������ڿ���ײ��״̬����ֻ���ø��෽��
	if (CanCollide())
	{
		//�����ڵ�����ʱ�Ķ�����Ⱦ
		Bullet::OnRender(_renderer);
		return;
	}

	#pragma region ExplosionAnimRender
	//����˵���ڵ���ǰ���ڱ�ը��״̬����ʱ����Ⱦ��ը����
	//���ڱ�ը����֡�ĳߴ粻ͬ���ӵ��ߴ磬������Ҫ����ȷ����Ⱦλ��
	static SDL_Point _point;
	_point.x = (int)(position.x - damageRange / 2);
	_point.y = (int)(position.y - damageRange / 2);
	animExploding.OnRender(_renderer, _point, 0);
	#pragma endregion
}

void Shell::OnCollide(Enemy* _enemy)
{
	//����ԭ����Bullet::OnCollide(_enemy);��ֱ��ʹ���ӵ���ʧ�����ڵ�����Ҫ���ű�ը����������ʧ
	DisableCollide();

	//����������Ч
	AudioManager::Instance()->PlaySFX(SoundResID::Shell_Hit);
}

#endif
