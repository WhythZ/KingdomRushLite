#ifndef _CONFIG_MANAGER_HPP_
#define _CONFIG_MANAGER_HPP_

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cJSON.h>
#include <SDL.h>
#include "Manager.hpp"
#include "../Map/Map.hpp"
#include "../Enemy/Wave.hpp"

//���ڴ�����Ϸ�����ļ�config.json�ڵ�Ԥ������Ϣ
class ConfigManager : public Manager<ConfigManager>
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
		std::string windowTitle = "KingdomRush-Lite";
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
		double viewRange[10] = { 5 };
		//���컨�ѣ�Ϊʲô�����飿��
		double buildCost[10] = { 50 };
		//��������
		double upgradeCost[10] = { 75 };
	};

	//����Ԥ����
	struct EnemyConfigPrefab
	{
		//����Ѫ��
		double hp = 100;
		//�����ƶ��ٶ�
		double speed = 1;
		//������ɵ��˺�
		double damage = 1;
		//���˱���ҵĸ���
		double coinRatio = 0.5;
		//���˻ָ����ܵ���ȴ����Χ��-1Ϊ���ָ���0Ϊֻ�ָ��Լ�����ָ�ǿ��
		double recoverCooldown = 10;
		double recoverRange = -1;
		double recoverIntensity = 25;
	};

public:
	const int initCoinNum = 100;           //��ʼ�Ľ����������
	const int pickCoinNum = 10;            //ÿʰȡһ��������ӵĽ����������
	const double initHomeHP = 10;          //�ҵĳ�ʼĬ��Ѫ������

	Map map;                               //��Ϸ��ͼ
	SDL_Rect mapRect = { 0 };              //��ͼ��Ⱦ�Ĵ���λ��

	bool isNotLose = true;                 //��û�����ƣ���Ϸ��û����
	bool isWavesOver = false;              //����ȫ������������Ϸ����

	int levelArcher;                       //�������ȼ���ȫ��ͳһ������
	int levelAxeman;                       //Ͷ���ֵȼ�
	int levelGunner;                       //ǹ���ֵȼ�

	std::vector<Wave> waveList;            //���ﲨ���б�

	BasicConfigPrefab basicConfigPrefab;
	PlayerConfigPrefab playerPrefab;

	TowerConfigPrefab archerPrefab;
	TowerConfigPrefab axemanPrefab;
	TowerConfigPrefab gunnerPrefab;

	EnemyConfigPrefab slimePrefab;
	EnemyConfigPrefab kingSlimePrefab;
	EnemyConfigPrefab skeletonPrefab;
	EnemyConfigPrefab goblinPrefab;
	EnemyConfigPrefab priestgGoblinPrefab;

public:
	bool LoadConfig(const std::string&);   //������Ϸ�ĸ���������ã�json��
	bool LoadLevel(const std::string&);    //���عؿ����εľ�����Ϣ��json��

private:
	ConfigManager() = default;
	~ConfigManager() = default;
};

bool ConfigManager::LoadConfig(const std::string& _path)
{
	#pragma region GetJsonRoot
	//�Ӵ���·����ȡ�ļ������ж��Ƿ��ȡʧ��
	std::ifstream _file(_path);
	if (!_file.good()) return false;

	//���ļ�����һ���Զ�ȡ������Ȼ��ر��ļ�
	std::stringstream _strStream;
	_strStream << _file.rdbuf();
	_file.close();
	
	//��_strStream������ת��ΪC�����ַ��������У�����json�ļ����������ݽ������洢�������ڣ�Ȼ�����Ƿ�����ɹ�
	cJSON* _jsonRoot = cJSON_Parse(_strStream.str().c_str());
	if (!_jsonRoot) return false;
	#pragma endregion

	//һ������ָ���ӽڵ�json����Ŀɸ���ָ��
	cJSON* _jsonChild = nullptr;

	//��ȡ��json�ı����ĵ�һ���ӽڵ�����
	_jsonChild = cJSON_GetObjectItem(_jsonRoot, "basic");
	basicConfigPrefab.windowWidth = cJSON_GetObjectItem(_jsonChild, "window_width")->valueint;
	basicConfigPrefab.windowHeight = cJSON_GetObjectItem(_jsonChild, "window_height")->valueint;
	basicConfigPrefab.windowTitle = cJSON_GetObjectItem(_jsonChild, "window_title")->valuestring;

	//��ȡ��json�ı����ĵڶ����ӽڵ�����
	_jsonChild = cJSON_GetObjectItem(_jsonRoot, "player");
	playerPrefab.speed = cJSON_GetObjectItem(_jsonChild, "speed")->valuedouble;
	playerPrefab.normalAttackCooldown = cJSON_GetObjectItem(_jsonChild, "normal_attack_cooldown")->valuedouble;
	playerPrefab.normalAttackDamage = cJSON_GetObjectItem(_jsonChild, "normal_attack_damage")->valuedouble;
	playerPrefab.skillCooldown = cJSON_GetObjectItem(_jsonChild, "skill_cooldown")->valuedouble;
	playerPrefab.skillDamage = cJSON_GetObjectItem(_jsonChild, "skill_damage")->valuedouble;

	//��ȡ��json�ı����ĵ������ӽڵ�����
	_jsonChild = cJSON_GetObjectItem(_jsonRoot, "tower");


	//��ȡ��json�ı����ĵ��ļ��ӽڵ�����
	_jsonChild = cJSON_GetObjectItem(_jsonRoot, "enemy_type");
}

bool ConfigManager::LoadLevel(const std::string& _path)
{
	#pragma region GetJsonRoot
	//�Ӵ���·����ȡ�ļ������ж��Ƿ��ȡʧ��
	std::ifstream _file(_path);
	if (!_file.good()) return false;

	//���ļ�����һ���Զ�ȡ������Ȼ��ر��ļ�
	std::stringstream _strStream;
	_strStream << _file.rdbuf();
	_file.close();

	//��_strStream������ת��ΪC�����ַ��������У�����json�ļ����������ݽ������洢�������ڣ�Ȼ�����Ƿ�����ɹ�
	cJSON* _jsonRoot = cJSON_Parse(_strStream.str().c_str());
	if (!_jsonRoot) return false;

	//�ؿ����ɶ�����Σ�Wave�������ɵ����飬����Ҫ���ļ��ı�������ʽ�ϣ�������������/�������Ƕ��󣩵ļ�⣬�����޷����к����Ľ���
	if (_jsonRoot->type != cJSON_Array)
	{
		//���ٺ��ٽ�����������ֹ�ڴ�й©
		cJSON_Delete(_jsonRoot);
		return false;
	}
	#pragma endregion

	//����ָ��ͬ���ε�json����Ŀɸ���ָ��
	cJSON* _jsonWave = nullptr;
	//json��������ĵ���������ǰ�߱������ߣ����飩�е�json����Ԫ��
	cJSON_ArrayForEach(_jsonWave, _jsonRoot)
	{
		//���õ��Ķ�������ͽ����жϣ��˴���Ҫ�ǻ����ŵ�json���������Ǿ������˶���
		if (_jsonWave->type != cJSON_Object)
			continue;

		//�õ������������б�β���Ŀ�Ԫ�أ�׼������¼���ѭ����ȡ���Ĳ�����Ϣ
		waveList.emplace_back();
		Wave& _wave = waveList.back();
		
		//�Ե�һ��ֵ��ӦΪdouble���֣����м��
		cJSON* _jsonRewards = cJSON_GetObjectItem(_jsonWave, "rewards");
		if (_jsonRewards && _jsonRewards->type == cJSON_Number)
			_wave.rewards = _jsonRewards->valuedouble;
		//�Եڶ���ֵ��ӦΪdouble���֣����м��
		cJSON* _jsonWaveInterval = cJSON_GetObjectItem(_jsonWave, "interval");
		if (_jsonWaveInterval && _jsonWaveInterval->type == cJSON_Number)
			_wave.interval = _jsonWaveInterval->valuedouble;
		//�Ե�����ֵ��ӦΪ�����¼������飩
		cJSON* _jsonSpawnList = cJSON_GetObjectItem(_jsonWave, "spawn_list");
		if (_jsonSpawnList && _jsonSpawnList->type == cJSON_Array)
		{
			//�����������еĶ���
			cJSON* _jsonSpawnEvent = nullptr;
			cJSON_ArrayForEach(_jsonSpawnEvent, _jsonSpawnList)
			{
				//����õ��Ķ��������
				if (_jsonWave->type != cJSON_Object)
					continue;

				//�õ�_wave.spawnEventListĩβ�����Ŀ�Ԫ�أ�׼�������������¼��
				_wave.spawnEventList.emplace_back();
				Wave::SpawnEvent& _spawnEvent = _wave.spawnEventList.back();
			
				//����һ��ֵ��ӦΪdouble��
				cJSON* _jsonSEventInterval = cJSON_GetObjectItem(_jsonSpawnEvent, "interval");
				if (_jsonSEventInterval && _jsonSEventInterval->type == cJSON_Number)
					_spawnEvent.interval = _jsonSEventInterval->valuedouble;
				//���ڶ���ֵ��ӦΪint��
				cJSON* _jsonSpawnPoint = cJSON_GetObjectItem(_jsonSpawnEvent, "spawn_point");
				if (_jsonSpawnPoint && _jsonSpawnPoint->type == cJSON_Number)
					_spawnEvent.spawnPoint = _jsonSpawnPoint->valueint;
				//��������ֵ��ӦΪstring��
				cJSON* _jsonEnemyType = cJSON_GetObjectItem(_jsonSpawnEvent, "enemy_type");
				if (_jsonEnemyType && _jsonEnemyType->type == cJSON_String)
				{
					if (_jsonEnemyType->valuestring == "Slime") { _spawnEvent.enemyType = EnemyType::Slime; }
					else if (_jsonEnemyType->valuestring == "KingSlime") { _spawnEvent.enemyType = EnemyType::KingSlime; }
					else if (_jsonEnemyType->valuestring == "Skeleton") { _spawnEvent.enemyType = EnemyType::Skeleton; }
					else if (_jsonEnemyType->valuestring == "Goblin") { _spawnEvent.enemyType = EnemyType::Goblin; }
					else if (_jsonEnemyType->valuestring == "PriestGoblin") { _spawnEvent.enemyType = EnemyType::PriestGoblin; }
				}

				////��ֹ�ڴ�й©
				//cJSON_Delete(_jsonSEventInterval);
				//cJSON_Delete(_jsonSpawnPoint);
				//cJSON_Delete(_jsonEnemyType);
			}
		}

		////��ֹ�ڴ�й©
		//cJSON_Delete(_jsonRewards);
		//cJSON_Delete(_jsonWaveInterval);
		//cJSON_Delete(_jsonSpawnList);

		//�����������ֵ�е����һ���������¼��б�Ϊ�գ���˵����_wave��ȡʧ�ܣ�ǰ���߶���Ĭ��ֵ����ȡʧ�ܾͲ�ȡĬ��ֵ������Ҫ����waveList�е���
		if (_wave.spawnEventList.empty())
			waveList.pop_back();
	}

	//��ֹ�ڴ�й©
	cJSON_Delete(_jsonWave);

	//���waveList�Ƿ�Ϊ�գ�Ϊ����˵�������ļ���д����
	if (waveList.empty()) return false;
	//һ�ж�ûë���Ļ��������շ��سɹ����ź�
	return true;
}

#endif
