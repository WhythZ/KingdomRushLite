#include "../../../Header/Tower/Concrete/Gunner.h"
#include "../../../Header/Manager/Concrete/AudioManager.h"

Gunner::Gunner()
{
	//指定防御塔类型
	type = TowerType::Gunner;
	//设置防御塔尺寸
	size.x = TILE_SIZE;
	size.y = TILE_SIZE;

	#pragma region SetBullet
	//设置子弹类型
	bulletType = BulletType::Shell;
	//设置子弹发射速度
	bulletSpeed = TOWER_GUNNER_BULLET_SPEED;
	#pragma endregion

	#pragma region SetAnimation
	//获取并设置防御塔的各动画帧图片
	static SDL_Texture* _sprites = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Tower_Gunner)->second;
	//根据该纹理资源图片的构造，此处的闲置状态动画第三、四帧为空，所以要避开类似2与3、6与7之类的索引
	static const std::vector<int> _idleUpIdicies = { 0, 1 };
	static const std::vector<int> _idleDownIdicies = { 4, 5 };
	static const std::vector<int> _idleLeftIdicies = { 8, 9 };
	static const std::vector<int> _idleRightIdicies = { 12, 13 };
	static const std::vector<int> _fireUpIdicies = { 16, 17, 18, 19 };
	static const std::vector<int> _fireDownIdicies = { 20, 21, 22, 23 };
	static const std::vector<int> _fireLeftIdicies = { 24, 25, 26, 27 };
	static const std::vector<int> _fireRightIdicies = { 28, 29, 30, 31 };

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

	//播放特定的发射音效
	AudioManager::Instance()->PlaySFX(SoundResID::Bullet_Shell_Fire);
}
