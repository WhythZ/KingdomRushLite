#ifndef _GUNNER_HPP_
#define _GUNNER_HPP_

#include "../Tower.hpp"

class Gunner : public Tower
{
public:
	Gunner();
	~Gunner() = default;

protected:
	void UpdateBulletData() override;    //实现基类中关于更新子弹属性的纯虚函数
	void OnFireBullet() override;        //重写基类中关于子弹发射的逻辑
};

Gunner::Gunner()
{
	//指定防御塔类型
	type = TowerType::Gunner;
}

void Gunner::UpdateBulletData()
{
	Tower::UpdateBulletData();

	//获取配置管理器
	static ConfigManager* _config = ConfigManager::Instance();

	//依据当前防御塔等级，获取对应的攻击间隔、攻击半径、伤害
	fireCooldown = _config->gunnerPrefab.cooldown[_config->levelGunner];
	fireRadius = _config->gunnerPrefab.viewRange[_config->levelGunner];
	bulletDamage = _config->gunnerPrefab.damage[_config->levelGunner];
}

void Gunner::OnFireBullet()
{
	Tower::OnFireBullet();

	//播放特定的发射音效
	AudioManager::Instance()->PlaySFX(SoundResID::Shell_Shoot);
}

#endif
