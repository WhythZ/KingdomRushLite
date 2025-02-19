#include "../../../Header/Tower/Concrete/Gunner.h"
#include "../../../Header/Manager/Concrete/AudioManager.h"

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
	bulletSpeed = TOWER_GUNNER_BULLET_SPEED;
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

	animIdleUp.SetAnimFrames(_sprites, 4, 8, _idleUpIdicies);
	animIdleDown.SetAnimFrames(_sprites, 4, 8, _idleDownIdicies);
	animIdleLeft.SetAnimFrames(_sprites, 4, 8, _idleLeftIdicies);
	animIdleRight.SetAnimFrames(_sprites, 4, 8, _idleRightIdicies);
	animFireUp.SetAnimFrames(_sprites, 4, 8, _fireUpIdicies);
	animFireDown.SetAnimFrames(_sprites, 4, 8, _fireDownIdicies);
	animFireLeft.SetAnimFrames(_sprites, 4, 8, _fireLeftIdicies);
	animFireRight.SetAnimFrames(_sprites, 4, 8, _fireRightIdicies);
	#pragma endregion
}

void Gunner::OnFireBullet(Enemy* _target)
{
	Tower::OnFireBullet(_target);

	//�����ض��ķ�����Ч
	AudioManager::Instance()->PlaySFX(SoundResID::Shell_Shoot);
}
