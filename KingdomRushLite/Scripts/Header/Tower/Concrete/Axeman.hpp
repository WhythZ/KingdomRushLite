#ifndef _AXEMAN_HPP_
#define _AXEMAN_HPP_

#include "../Tower.hpp"

class Axeman : public Tower
{
public:
	Axeman();
	~Axeman() = default;

protected:
	void UpdateBulletData() override;    //ʵ�ֻ����й��ڸ����ӵ����ԵĴ��麯��
	void OnFireBullet() override;        //��д�����й����ӵ�������߼�
};

Axeman::Axeman()
{
	//ָ������������
	type = TowerType::Axeman;
}

void Axeman::UpdateBulletData()
{
	Tower::UpdateBulletData();

	//��ȡ���ù�����
	static ConfigManager* _config = ConfigManager::Instance();

	//���ݵ�ǰ�������ȼ�����ȡ��Ӧ�Ĺ�������������뾶���˺�
	fireCooldown = _config->axemanPrefab.cooldown[_config->levelAxeman];
	fireRadius = _config->axemanPrefab.viewRange[_config->levelAxeman];
	bulletDamage = _config->axemanPrefab.damage[_config->levelAxeman];
}

void Axeman::OnFireBullet()
{
	Tower::OnFireBullet();

	//�����ض��ķ�����Ч
	AudioManager::Instance()->PlaySFX(SoundResID::Axe_Shoot);
}

#endif
