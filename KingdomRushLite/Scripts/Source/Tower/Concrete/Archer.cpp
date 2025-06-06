#include "../../../Header/Tower/Concrete/Archer.h"
#include "../../../Header/Manager/Concrete/AudioManager.h"

Archer::Archer()
{
	//指定防御塔类型
	type = TowerType::Archer;

	//设置防御塔尺寸
	size.x = TILE_SIZE;
	size.y = TILE_SIZE;

	#pragma region SetBullet
	//设置子弹类型
	bulletType = BulletType::Arrow;
	//设置子弹发射速度
	bulletSpeed = TOWER_ARCHER_BULLET_SPEED;
	#pragma endregion

	#pragma region SetAnimation
	//获取并设置防御塔的各动画帧图片
	static SDL_Texture* _sprites = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Tower_Archer)->second;
	//根据该纹理资源图片的构造，此处的闲置状态动画第三帧为空，所以要避开类似2、5之类的索引
	static const std::vector<int> _idleUpIdicies = { 0, 1 };
	static const std::vector<int> _idleDownIdicies = { 3, 4 };
	static const std::vector<int> _idleLeftIdicies = { 6, 7 };
	static const std::vector<int> _idleRightIdicies = { 9, 10 };
	static const std::vector<int> _fireUpIdicies = { 12, 13, 14 };
	static const std::vector<int> _fireDownIdicies = { 15, 16, 17 };
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

void Archer::OnFireBullet(Enemy* _target)
{
	Tower::OnFireBullet(_target);

	//播放特定的发射音效
	switch (rand() % 2)
	{
	case 0:
		AudioManager::Instance()->PlaySFX(SoundResID::Bullet_Arrow_Fire_1);
		break;
	case 1:
		AudioManager::Instance()->PlaySFX(SoundResID::Bullet_Arrow_Fire_2);
		break;
	default:
		break;
	}
}
