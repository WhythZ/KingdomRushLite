#ifndef _ARCHER_HPP_
#define _ARCHER_HPP_

#include "../Tower.hpp"

#define ARCHER_BULLET_SPEED 6

class Archer : public Tower
{
public:
	Archer();
	~Archer() = default;

protected:
	void OnFireBullet() override;        //��д�����й����ӵ�������߼�
};

Archer::Archer()
{
	//ָ������������
	type = TowerType::Archer;
	//���÷������ߴ�
	size.x = 48; size.y = 48;

	#pragma region Bullet
	//�����ӵ�����
	bulletType = BulletType::Arrow;
	//�����ӵ������ٶ�
	bulletSpeed = ARCHER_BULLET_SPEED;
	#pragma endregion

	#pragma region Animation
	//��ȡ�����÷������ĸ�����֡ͼƬ
	static SDL_Texture* _sprites = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Tower_Archer)->second;
	//���ݸ�������ԴͼƬ�Ĺ��죬�˴�������״̬��������֡Ϊ�գ�����Ҫ�ܿ�����2��5֮�������
	static const std::vector<int> _idleUpIdicies = { 3, 4 };
	static const std::vector<int> _idleDownIdicies = { 0, 1 };
	static const std::vector<int> _idleLeftIdicies = { 6, 7 };
	static const std::vector<int> _idleRightIdicies = { 9, 10 };
	static const std::vector<int> _fireUpIdicies = { 15, 16, 17 };
	static const std::vector<int> _fireDownIdicies = { 12, 13, 14 };
	static const std::vector<int> _fireLeftIdicies = { 18, 19, 20 };
	static const std::vector<int> _fireRightIdicies = { 21, 22, 23 };

	animIdleUp.SetAnimFrames(_sprites, 3, 8, _idleUpIdicies);
	animIdleDown.SetAnimFrames(_sprites, 3, 8, _idleDownIdicies);
	animIdleLeft.SetAnimFrames(_sprites, 3, 8, _idleLeftIdicies);
	animIdleRight.SetAnimFrames(_sprites, 3, 8, _idleRightIdicies);
	animFireUp.SetAnimFrames(_sprites, 3, 8, _fireUpIdicies);
	animFireDown.SetAnimFrames(_sprites, 3, 8, _fireDownIdicies);
	animFireLeft.SetAnimFrames(_sprites, 3, 8, _fireLeftIdicies);
	animFireRight.SetAnimFrames(_sprites, 3, 8, _fireRightIdicies);
	#pragma endregion
}

void Archer::OnFireBullet()
{
	Tower::OnFireBullet();

	//�����ض��ķ�����Ч
	switch (rand() % 2)
	{
	case 0:
		AudioManager::Instance()->PlaySFX(SoundResID::Arrow_Shoot_1);
		break;
	case 1:
		AudioManager::Instance()->PlaySFX(SoundResID::Arrow_Shoot_2);
		break;
	default:
		break;
	}
}

#endif
