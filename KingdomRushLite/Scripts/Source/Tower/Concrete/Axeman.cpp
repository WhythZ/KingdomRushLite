#include "../../../Header/Tower/Concrete/Axeman.h"
#include "../../../Header/Manager/Concrete/AudioManager.h"

Axeman::Axeman()
{
	//ָ������������
	type = TowerType::Axeman;
	//���÷������ߴ�
	size.x = 48; size.y = 48;

	#pragma region Bullet
	//�����ӵ�����
	bulletType = BulletType::Axe;
	//�����ӵ������ٶ�
	bulletSpeed = AXEMAN_BULLET_SPEED;
	#pragma endregion

	#pragma region Animation
	//��ȡ�����÷������ĸ�����֡ͼƬ
	static SDL_Texture* _sprites = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Tower_Axeman)->second;
	//���ݸ�������ԴͼƬ�Ĺ��죬�˴�������״̬��������֡Ϊ�գ�����Ҫ�ܿ�����2��5֮�������
	static const std::vector<int> _idleUpIdicies = { 3, 4 };
	static const std::vector<int> _idleDownIdicies = { 0, 1 };
	static const std::vector<int> _idleLeftIdicies = { 9, 10 };
	static const std::vector<int> _idleRightIdicies = { 6, 7 };
	static const std::vector<int> _fireUpIdicies = { 15, 16, 17 };
	static const std::vector<int> _fireDownIdicies = { 12, 13, 14 };
	static const std::vector<int> _fireLeftIdicies = { 21, 22, 23 };
	static const std::vector<int> _fireRightIdicies = { 18, 19, 20 };

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

void Axeman::OnFireBullet()
{
	Tower::OnFireBullet();

	//�����ض��ķ�����Ч
	AudioManager::Instance()->PlaySFX(SoundResID::Axe_Shoot);
}
