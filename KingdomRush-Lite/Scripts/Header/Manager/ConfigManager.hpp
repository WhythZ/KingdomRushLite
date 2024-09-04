#ifndef _CONFIG_MANAGER_HPP_
#define _CONFIG_MANAGER_HPP_

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cJSON.h>
#include <SDL.h>
#include "Manager.hpp"

//���ڴ�����Ϸ�����ļ�config.json�ڵ�Ԥ������Ϣ
class ConfigManager : public Manager<ConfigManager>
{
	friend class Manager<ConfigManager>;

public:
	//��Ϸ�Ļ���������ϢԤ����
	struct BasicConfigInfoPrefab
	{
		//���ڱ���
		std::string windowTitle = "KingdomRush-Lite";
		//���ڸ߶�����
		int windowWidth = 1280;
		int windowHeight = 720;
	};

	//���Ԥ����
	struct PlayerPrefab
	{
		//����ƶ��ٶ�
		double speed = 3;
		//����չ�
		double normalAttackCooldown = 0.5;
		double normalAttackDamage = 0;
		//��Ҽ���
		double skillCooldown = 10;
		double skillDamage = 0;
	};

	//������Ԥ����
	struct TowerPrefab
	{
		//������ÿһ���Ĺ������������洢
		double cooldown[10] = { 1 };
		//�����˺�
		double damage[10] = { 25 };
		//��Ұ��Χ
		double viewRange[10] = { 5 };
		//���컨�ѣ�Ϊʲô�����飿��
		double buildCost[10] = { 50 };
		//��������
		double upgradeCost[10] = { 75 };
	};

	//����Ԥ����
	struct EnemyPrefab
	{
		//����Ѫ��
		double hp = 100;
		//�����ƶ��ٶ�
		double speed = 1;
		//������ɵ��˺�
		double damage = 1;
		//���˱���ҵĸ���
		double coinRation = 0.5;
		//���˻ָ����ܵ���ȴ����Χ��-1Ϊ���ָ���0Ϊֻ�ָ��Լ�����ָ�ǿ��
		double recoverCooldown = 10;
		double recoverRange = -1;
		double recoverIntensity = 25;
	};

protected:
	ConfigManager() = default;

	~ConfigManager() = default;
};

#endif
