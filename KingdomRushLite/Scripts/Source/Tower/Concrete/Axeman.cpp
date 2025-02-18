#include "../../../Header/Tower/Concrete/Axeman.h"
#include "../../../Header/Manager/Concrete/AudioManager.h"

Axeman::Axeman()
{
	//指定防御塔类型
	type = TowerType::Axeman;
	//设置防御塔尺寸
	size.x = 48; size.y = 48;

	#pragma region Bullet
	//设置子弹类型
	bulletType = BulletType::Axe;
	//设置子弹发射速度
	bulletSpeed = AXEMAN_BULLET_SPEED;
	#pragma endregion

	#pragma region Animation
	//获取并设置防御塔的各动画帧图片
	static SDL_Texture* _sprites = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Tower_Axeman)->second;
	//根据该纹理资源图片的构造，此处的闲置状态动画第三帧为空，所以要避开类似2、5之类的索引
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

	//播放特定的发射音效
	AudioManager::Instance()->PlaySFX(SoundResID::Axe_Shoot);
}
