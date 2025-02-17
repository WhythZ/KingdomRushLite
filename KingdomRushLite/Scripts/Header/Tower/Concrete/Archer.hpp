#ifndef _ARCHER_HPP_
#define _ARCHER_HPP_

#include "../Tower.hpp"

class Archer : public Tower
{
public:
	Archer();
	~Archer() = default;

protected:
	void UpdateBulletData() override;    //实现基类中关于更新子弹属性的纯虚函数
	void OnFireBullet() override;        //重写基类中关于子弹发射的逻辑
};

Archer::Archer()
{
	//指定防御塔类型
	type = TowerType::Archer;
}

void Archer::UpdateBulletData()
{
	Tower::UpdateBulletData();

	//获取配置管理器
	static ConfigManager* _config = ConfigManager::Instance();

	//依据当前防御塔等级，获取对应的攻击间隔、攻击半径、伤害
	fireCooldown = _config->archerPrefab.cooldown[_config->levelArcher];
	fireRadius = _config->archerPrefab.viewRange[_config->levelArcher];
	bulletDamage = _config->archerPrefab.damage[_config->levelArcher];
}

void Archer::OnFireBullet()
{
	Tower::OnFireBullet();

	//播放特定的发射音效
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
