#ifndef _AXEMAN_HPP_
#define _AXEMAN_HPP_

#include "../Tower.hpp"

class Axeman : public Tower
{
public:
	Axeman();
	~Axeman() = default;

protected:
	void UpdateBulletData() override;    //实现基类中关于更新子弹属性的纯虚函数
	void OnFireBullet() override;        //重写基类中关于子弹发射的逻辑
};

Axeman::Axeman()
{
	//指定防御塔类型
	type = TowerType::Axeman;
}

void Axeman::UpdateBulletData()
{
	Tower::UpdateBulletData();

	//获取配置管理器
	static ConfigManager* _config = ConfigManager::Instance();

	//依据当前防御塔等级，获取对应的攻击间隔、攻击半径、伤害
	fireCooldown = _config->axemanPrefab.cooldown[_config->levelAxeman];
	fireRadius = _config->axemanPrefab.viewRange[_config->levelAxeman];
	bulletDamage = _config->axemanPrefab.damage[_config->levelAxeman];
}

void Axeman::OnFireBullet()
{
	Tower::OnFireBullet();

	//播放特定的发射音效
	AudioManager::Instance()->PlaySFX(SoundResID::Axe_Shoot);
}

#endif
