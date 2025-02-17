#ifndef _GUNNER_HPP_
#define _GUNNER_HPP_

#include "../Tower.hpp"

#define GUNNER_BULLET_SPEED 6

class Gunner : public Tower
{
public:
	Gunner();
	~Gunner() = default;

protected:
	void UpdateBulletData() override;    //ʵ�ֻ����й��ڸ����ӵ����ԵĴ��麯��
	void OnFireBullet() override;        //��д�����й����ӵ�������߼�
};

Gunner::Gunner()
{
	//ָ������������
	type = TowerType::Gunner;
	//���÷������ߴ�
	size.x = 48; size.y = 48;

	#pragma region Bullet
	//�����ӵ�����
	bulletType = BulletType::Shell;
	//�����ӵ������ٶ�
	bulletSpeed = GUNNER_BULLET_SPEED;
	#pragma endregion

	#pragma region Animation
	//��ȡ�����÷������ĸ�����֡ͼƬ
	static SDL_Texture* _sprites = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Tower_Gunner)->second;
	//���ݸ�������ԴͼƬ�Ĺ��죬�˴�������״̬������������֡Ϊ�գ�����Ҫ�ܿ�����2��3��6��7֮�������
	static const std::vector<int> _idleUpIdicies = { 4, 5 };
	static const std::vector<int> _idleDownIdicies = { 0, 1 };
	static const std::vector<int> _idleLeftIdicies = { 12, 13 };
	static const std::vector<int> _idleRightIdicies = { 8, 9 };
	static const std::vector<int> _fireUpIdicies = { 20, 21, 22, 23 };
	static const std::vector<int> _fireDownIdicies = { 16, 17, 18, 19 };
	static const std::vector<int> _fireLeftIdicies = { 28, 29, 30, 31 };
	static const std::vector<int> _fireRightIdicies = { 24, 25, 26, 27 };

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

void Gunner::UpdateBulletData()
{
	Tower::UpdateBulletData();

	//��ȡ���ù�����
	static ConfigManager* _config = ConfigManager::Instance();

	//���ݵ�ǰ�������ȼ�����ȡ��Ӧ�Ĺ�������������뾶���˺�
	fireCooldown = _config->gunnerPrefab.cooldown[_config->levelGunner];
	fireRadius = _config->gunnerPrefab.viewRange[_config->levelGunner] * TILE_SIZE;
	bulletDamage = _config->gunnerPrefab.damage[_config->levelGunner];
}

void Gunner::OnFireBullet()
{
	Tower::OnFireBullet();

	//�����ض��ķ�����Ч
	AudioManager::Instance()->PlaySFX(SoundResID::Shell_Shoot);
}

#endif
