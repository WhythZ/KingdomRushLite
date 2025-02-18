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
	void OnFireBullet() override;        //重写基类中关于子弹发射的逻辑
};

Gunner::Gunner()
{
	//指定防御塔类型
	type = TowerType::Gunner;
	//设置防御塔尺寸
	size.x = 48; size.y = 48;

	#pragma region Bullet
	//设置子弹类型
	bulletType = BulletType::Shell;
	//设置子弹发射速度
	bulletSpeed = GUNNER_BULLET_SPEED;
	#pragma endregion

	#pragma region Animation
	//获取并设置防御塔的各动画帧图片
	static SDL_Texture* _sprites = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Tower_Gunner)->second;
	//根据该纹理资源图片的构造，此处的闲置状态动画第三、四帧为空，所以要避开类似2与3、6与7之类的索引
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

void Gunner::OnFireBullet()
{
	Tower::OnFireBullet();

	//播放特定的发射音效
	AudioManager::Instance()->PlaySFX(SoundResID::Shell_Shoot);
}

#endif
