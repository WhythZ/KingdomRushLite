#ifndef _CONFIG_MANAGER_H_
#define _CONFIG_MANAGER_H_

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cJSON.h>
#include <SDL.h>
#include "../Manager.hpp"
#include "../../Enemy/Wave.h"

//��������ߵȼ�
#define TOWER_MAX_LEVEL 10

//���ڴ�����Ϸ�����ļ�config.json�ڵ�Ԥ������Ϣ
class ConfigManager :public Manager<ConfigManager>
{
	friend class Manager<ConfigManager>;

public:
	//��Ϸ�Ļ���������ϢԤ����
	struct BasicConfigPrefab
	{
		//���ڸ߶�����
		int windowWidth = 1280;
		int windowHeight = 720;
		//���ڱ���
		std::string windowTitle = "KingdomRushLite";
	};

	//���Ԥ����
	struct PlayerConfigPrefab
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
	struct TowerConfigPrefab
	{
		//������ÿһ���Ĺ������������洢
		double cooldown[10] = { 1 };
		//�����˺�
		double damage[10] = { 25 };
		//��Ұ��Χ
		double fireRadius[10] = { 5 };
		//���컨��
		double buildCost[10] = { 50 };
		//��������
		double upgradeCost[10] = { 75 };
	};

	//����Ԥ����
	struct EnemyConfigPrefab
	{
		//����Ѫ��
		double health = 100;
		//�����ƶ��ٶ�
		double speed = 1;
		//������ɵ��˺�
		double damage = 1;
		//���˱���ҵĸ���
		double coinRatio = 0.5;
		//���˻ָ����ܵ���ȴ����Χ�뾶��-1Ϊ���ָ���0Ϊֻ�ָ��Լ�����ָ�ǿ��
		double skillRecoverCooldown = 10;
		double skillRecoverRadius = -1;
		double skillRecoverIntensity = 25;
	};

public:
	#pragma region LoadedSettings
	BasicConfigPrefab basicPrefab;          //�洢����������Ϣ
	PlayerConfigPrefab playerDragonPrefab;  //�洢���������Ϣ

	std::vector<Wave> waveList;             //���ﲨ���б�

	TowerConfigPrefab archerPrefab;         //�洢������������Ϣ
	TowerConfigPrefab axemanPrefab;         //�洢Ͷ����������Ϣ
	TowerConfigPrefab gunnerPrefab;         //�洢ǹ����������Ϣ

	EnemyConfigPrefab slimePrefab;          //�洢ʷ��ķ������Ϣ
	EnemyConfigPrefab slimeKingPrefab;      //�洢ʷ��ķ��������Ϣ
	EnemyConfigPrefab skeletonPrefab;       //�洢����������Ϣ
	EnemyConfigPrefab goblinPrefab;         //�洢�粼��������Ϣ
	EnemyConfigPrefab goblinPriestPrefab;   //�洢�粼�ּ�˾������Ϣ
	#pragma endregion

	const int initCoinNum = 100;            //��ʼ�Ľ����������
	const int pickCoinNum = 10;             //ÿʰȡһ��������ӵĽ����������

	const double initHomeHP = 10;           //�ҵĳ�ʼĬ��Ѫ������

public:
	bool LoadConfig(const std::string&);    //������Ϸ�ĸ���������ã�json��
	bool LoadWaves(const std::string&);     //���عؿ����εľ�����Ϣ��json��

private:
	ConfigManager() = default;
	~ConfigManager() = default;

	bool ParseBasicConfigPrefab(BasicConfigPrefab&, cJSON*);    //ר�Ž�������������Ϣ
	bool ParsePlayerConfigPrefab(PlayerConfigPrefab&, cJSON*);  //ר�Ž������������Ϣ
	bool ParseTowerConfigPrefab(TowerConfigPrefab&, cJSON*);    //ר�Ž���������������Ϣ
	bool ParseEnemyConfigPrefab(EnemyConfigPrefab&, cJSON*);    //ר�Ž�������������Ϣ

	bool ParseNumberArray(double*, int, cJSON*);                //�����������飬��������ĸ�������
};

#endif
