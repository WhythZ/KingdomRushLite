#ifndef _GUNNER_HPP_
#define _GUNNER_HPP_

#include "../Tower.hpp"

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
}

void Gunner::UpdateBulletData()
{
	Tower::UpdateBulletData();

	//��ȡ���ù�����
	static ConfigManager* _config = ConfigManager::Instance();

	//���ݵ�ǰ�������ȼ�����ȡ��Ӧ�Ĺ�������������뾶���˺�
	fireCooldown = _config->gunnerPrefab.cooldown[_config->levelGunner];
	fireRadius = _config->gunnerPrefab.viewRange[_config->levelGunner];
	bulletDamage = _config->gunnerPrefab.damage[_config->levelGunner];
}

void Gunner::OnFireBullet()
{
	Tower::OnFireBullet();

	//�����ض��ķ�����Ч
	AudioManager::Instance()->PlaySFX(SoundResID::Shell_Shoot);
}

#endif
