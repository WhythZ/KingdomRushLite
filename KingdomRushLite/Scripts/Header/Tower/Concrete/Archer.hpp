#ifndef _ARCHER_HPP_
#define _ARCHER_HPP_

#include "../Tower.hpp"

class Archer : public Tower
{
public:
	Archer();
	~Archer() = default;

protected:
	void UpdateBulletData() override;    //ʵ�ֻ����й��ڸ����ӵ����ԵĴ��麯��
	void OnFireBullet() override;        //��д�����й����ӵ�������߼�
};

Archer::Archer()
{
	//ָ������������
	type = TowerType::Archer;
}

void Archer::UpdateBulletData()
{
	Tower::UpdateBulletData();

	//��ȡ���ù�����
	static ConfigManager* _config = ConfigManager::Instance();

	//���ݵ�ǰ�������ȼ�����ȡ��Ӧ�Ĺ�������������뾶���˺�
	fireCooldown = _config->archerPrefab.cooldown[_config->levelArcher];
	fireRadius = _config->archerPrefab.viewRange[_config->levelArcher];
	bulletDamage = _config->archerPrefab.damage[_config->levelArcher];
}

void Archer::OnFireBullet()
{
	Tower::OnFireBullet();

	//�����ض��ķ�����Ч
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
