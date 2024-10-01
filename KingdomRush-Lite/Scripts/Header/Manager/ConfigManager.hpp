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

//用于处理游戏配置文件config.json内的预制体信息
class ConfigManager : public Manager<ConfigManager>
{
	friend class Manager<ConfigManager>;

public:
	//游戏的基础配置信息预制体
	struct BasicConfigPrefab
	{
		//窗口高度与宽度
		int windowWidth = 1280;
		int windowHeight = 720;
		//窗口标题
		std::string windowTitle = "KingdomRush-Lite";
	};

	//玩家预制体
	struct PlayerConfigPrefab
	{
		//玩家移动速度
		double speed = 3;
		//玩家普攻
		double normalAttackCooldown = 0.5;
		double normalAttackDamage = 0;
		//玩家技能
		double skillCooldown = 10;
		double skillDamage = 0;
	};

	//防御塔预制体
	struct TowerConfigPrefab
	{
		//防御塔每一级的攻击间隔，数组存储
		double cooldown[10] = { 1 };
		//攻击伤害
		double damage[10] = { 25 };
		//视野范围
		double viewRange[10] = { 5 };
		//建造花费（为什么是数组？）
		double buildCost[10] = { 50 };
		//升级花费
		double upgradeCost[10] = { 75 };
	};

	//敌人预制体
	struct EnemyConfigPrefab
	{
		//敌人血量
		double hp = 100;
		//敌人移动速度
		double speed = 1;
		//敌人造成的伤害
		double damage = 1;
		//敌人爆金币的概率
		double coinRatio = 0.5;
		//敌人恢复技能的冷却、范围（-1为不恢复，0为只恢复自己）与恢复强度
		double recoverCooldown = 10;
		double recoverRange = -1;
		double recoverIntensity = 25;
	};

public:
	const int initCoinNum = 100;           //初始的金币数量常量
	const int pickCoinNum = 10;            //每拾取一个金币增加的金币数量常量
	const double initHomeHP = 10;          //家的初始默认血量常量

	Map map;                               //游戏地图
	SDL_Rect mapRect = { 0 };              //地图渲染的窗口位置

	bool isNotLose = true;                 //家没被攻破，游戏就没有输
	bool isWavesOver = false;              //波次全部结束，则游戏结束

	int levelArcher;                       //弓箭塔等级（全局统一升级）
	int levelAxeman;                       //投斧手等级
	int levelGunner;                       //枪炮手等级

	std::vector<Wave> waveList;            //怪物波次列表

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
	bool LoadConfig(const std::string&);   //加载游戏的各项基本设置（json）
	bool LoadLevel(const std::string&);    //加载关卡波次的具体信息（json）

private:
	ConfigManager() = default;
	~ConfigManager() = default;
};

bool ConfigManager::LoadConfig(const std::string& _path)
{
	#pragma region GetJsonRoot
	//从传入路径读取文件，并判断是否读取失败
	std::ifstream _file(_path);
	if (!_file.good()) return false;

	//将文件内容一次性读取出来，然后关闭文件
	std::stringstream _strStream;
	_strStream << _file.rdbuf();
	_file.close();
	
	//将_strStream的内容转化为C风格的字符数组后进行，将该json文件的所有内容解析并存储到容器内，然后检测是否解析成功
	cJSON* _jsonRoot = cJSON_Parse(_strStream.str().c_str());
	if (!_jsonRoot) return false;
	#pragma endregion

	//一个用于指向子节点json对象的可复用指针
	cJSON* _jsonChild = nullptr;

	//读取该json文本树的第一级子节点内容
	_jsonChild = cJSON_GetObjectItem(_jsonRoot, "basic");
	basicConfigPrefab.windowWidth = cJSON_GetObjectItem(_jsonChild, "window_width")->valueint;
	basicConfigPrefab.windowHeight = cJSON_GetObjectItem(_jsonChild, "window_height")->valueint;
	basicConfigPrefab.windowTitle = cJSON_GetObjectItem(_jsonChild, "window_title")->valuestring;

	//读取该json文本树的第二级子节点内容
	_jsonChild = cJSON_GetObjectItem(_jsonRoot, "player");
	playerPrefab.speed = cJSON_GetObjectItem(_jsonChild, "speed")->valuedouble;
	playerPrefab.normalAttackCooldown = cJSON_GetObjectItem(_jsonChild, "normal_attack_cooldown")->valuedouble;
	playerPrefab.normalAttackDamage = cJSON_GetObjectItem(_jsonChild, "normal_attack_damage")->valuedouble;
	playerPrefab.skillCooldown = cJSON_GetObjectItem(_jsonChild, "skill_cooldown")->valuedouble;
	playerPrefab.skillDamage = cJSON_GetObjectItem(_jsonChild, "skill_damage")->valuedouble;

	//读取该json文本树的第三级子节点内容
	_jsonChild = cJSON_GetObjectItem(_jsonRoot, "tower");


	//读取该json文本树的第四级子节点内容
	_jsonChild = cJSON_GetObjectItem(_jsonRoot, "enemy_type");
}

bool ConfigManager::LoadLevel(const std::string& _path)
{
	#pragma region GetJsonRoot
	//从传入路径读取文件，并判断是否读取失败
	std::ifstream _file(_path);
	if (!_file.good()) return false;

	//将文件内容一次性读取出来，然后关闭文件
	std::stringstream _strStream;
	_strStream << _file.rdbuf();
	_file.close();

	//将_strStream的内容转化为C风格的字符数组后进行，将该json文件的所有内容解析并存储到容器内，然后检测是否解析成功
	cJSON* _jsonRoot = cJSON_Parse(_strStream.str().c_str());
	if (!_jsonRoot) return false;

	//关卡是由多个波次（Wave类对象）组成的数组，所以要对文件文本进行形式上（中括号是数组/花括号是对象）的检测，否则无法进行后续的解析
	if (_jsonRoot->type != cJSON_Array)
	{
		//销毁后再结束函数，防止内存泄漏
		cJSON_Delete(_jsonRoot);
		return false;
	}
	#pragma endregion

	//用于指向不同波次的json对象的可复用指针
	cJSON* _jsonWave = nullptr;
	//json对象数组的迭代器，用前者遍历后者（数组）中的json对象元素
	cJSON_ArrayForEach(_jsonWave, _jsonRoot)
	{
		//对拿到的对象的类型进行判断（此处需要是花括号的json对象）若不是就跳过此对象
		if (_jsonWave->type != cJSON_Object)
			continue;

		//拿到新增到波次列表尾部的空元素，准备对其录入此循环读取到的波次信息
		waveList.emplace_back();
		Wave& _wave = waveList.back();
		
		//对第一个值（应为double数字）进行检测
		cJSON* _jsonRewards = cJSON_GetObjectItem(_jsonWave, "rewards");
		if (_jsonRewards && _jsonRewards->type == cJSON_Number)
			_wave.rewards = _jsonRewards->valuedouble;
		//对第二个值（应为double数字）进行检测
		cJSON* _jsonWaveInterval = cJSON_GetObjectItem(_jsonWave, "interval");
		if (_jsonWaveInterval && _jsonWaveInterval->type == cJSON_Number)
			_wave.interval = _jsonWaveInterval->valuedouble;
		//对第三个值（应为生成事件的数组）
		cJSON* _jsonSpawnList = cJSON_GetObjectItem(_jsonWave, "spawn_list");
		if (_jsonSpawnList && _jsonSpawnList->type == cJSON_Array)
		{
			//遍历该数组中的对象
			cJSON* _jsonSpawnEvent = nullptr;
			cJSON_ArrayForEach(_jsonSpawnEvent, _jsonSpawnList)
			{
				//检测拿到的对象的类型
				if (_jsonWave->type != cJSON_Object)
					continue;

				//拿到_wave.spawnEventList末尾新增的空元素，准备对其进行数据录入
				_wave.spawnEventList.emplace_back();
				Wave::SpawnEvent& _spawnEvent = _wave.spawnEventList.back();
			
				//检测第一个值（应为double）
				cJSON* _jsonSEventInterval = cJSON_GetObjectItem(_jsonSpawnEvent, "interval");
				if (_jsonSEventInterval && _jsonSEventInterval->type == cJSON_Number)
					_spawnEvent.interval = _jsonSEventInterval->valuedouble;
				//检测第二个值（应为int）
				cJSON* _jsonSpawnPoint = cJSON_GetObjectItem(_jsonSpawnEvent, "spawn_point");
				if (_jsonSpawnPoint && _jsonSpawnPoint->type == cJSON_Number)
					_spawnEvent.spawnPoint = _jsonSpawnPoint->valueint;
				//检测第三个值（应为string）
				cJSON* _jsonEnemyType = cJSON_GetObjectItem(_jsonSpawnEvent, "enemy_type");
				if (_jsonEnemyType && _jsonEnemyType->type == cJSON_String)
				{
					if (_jsonEnemyType->valuestring == "Slime") { _spawnEvent.enemyType = EnemyType::Slime; }
					else if (_jsonEnemyType->valuestring == "KingSlime") { _spawnEvent.enemyType = EnemyType::KingSlime; }
					else if (_jsonEnemyType->valuestring == "Skeleton") { _spawnEvent.enemyType = EnemyType::Skeleton; }
					else if (_jsonEnemyType->valuestring == "Goblin") { _spawnEvent.enemyType = EnemyType::Goblin; }
					else if (_jsonEnemyType->valuestring == "PriestGoblin") { _spawnEvent.enemyType = EnemyType::PriestGoblin; }
				}

				////防止内存泄漏
				//cJSON_Delete(_jsonSEventInterval);
				//cJSON_Delete(_jsonSpawnPoint);
				//cJSON_Delete(_jsonEnemyType);
			}
		}

		////防止内存泄漏
		//cJSON_Delete(_jsonRewards);
		//cJSON_Delete(_jsonWaveInterval);
		//cJSON_Delete(_jsonSpawnList);

		//如果上述三个值中的最后一个（生成事件列表）为空，则说明该_wave读取失败（前两者都有默认值，读取失败就采取默认值），需要被从waveList中弹出
		if (_wave.spawnEventList.empty())
			waveList.pop_back();
	}

	//防止内存泄漏
	cJSON_Delete(_jsonWave);

	//检测waveList是否为空，为空则说明配置文件编写有误
	if (waveList.empty()) return false;
	//一切都没毛病的话，就最终返回成功的信号
	return true;
}

#endif
