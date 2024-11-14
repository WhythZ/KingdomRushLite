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

//防御塔最高等级
#define TOWER_MAX_LEVEL 10

//用于处理游戏配置文件config.json内的预制体信息
class ConfigManager :public Manager<ConfigManager>
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

	#pragma region LoadedPrefabs
	BasicConfigPrefab basicPrefab;         //存储基本配置信息
	PlayerConfigPrefab playerPrefab;       //存储玩家配置信息

	TowerConfigPrefab archerPrefab;
	TowerConfigPrefab axemanPrefab;
	TowerConfigPrefab gunnerPrefab;

	EnemyConfigPrefab slimePrefab;
	EnemyConfigPrefab slimeKingPrefab;
	EnemyConfigPrefab skeletonPrefab;
	EnemyConfigPrefab goblinPrefab;
	EnemyConfigPrefab goblinPriestPrefab;
	#pragma endregion

	#pragma region Map
	Map map;                               //游戏地图
	SDL_Rect mapRect = { 0 };              //地图矩形的左上顶点渲染在游戏窗口内的位置
	#pragma endregion

	const double initHomeHP = 10;          //家的初始默认血量常量
	std::vector<Wave> waveList;            //怪物波次列表

	#pragma region Tower
	int levelArcher = 0;                   //弓箭塔等级（全局统一升级）
	int levelAxeman = 0;                   //投斧手等级
	int levelGunner = 0;                   //枪炮手等级
	#pragma endregion

	#pragma region Coin
	const int initCoinNum = 100;           //初始的金币数量常量
	const int pickCoinNum = 10;            //每拾取一个金币增加的金币数量常量
	#pragma endregion

public:
	bool LoadConfig(const std::string&);   //加载游戏的各项基本设置（json）
	bool LoadLevel(const std::string&);    //加载关卡波次的具体信息（json）

private:
	ConfigManager() = default;
	~ConfigManager() = default;

	bool ParseBasicConfigPrefab(BasicConfigPrefab&, cJSON*);    //专门解析基本配置信息
	bool ParsePlayerConfigPrefab(PlayerConfigPrefab&, cJSON*);  //专门解析玩家配置信息
	bool ParseTowerConfigPrefab(TowerConfigPrefab&, cJSON*);    //专门解析防御塔配置信息
	bool ParseEnemyConfigPrefab(EnemyConfigPrefab&, cJSON*);    //专门解析敌人配置信息

	bool ParseNumberArray(double*, int, cJSON*);                //解析数字数组，如防御塔的各级属性
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

	//将_strStream的内容转化为C风格字符数组后解析并存储（cJSON_Parse方法是开辟到堆区的，故后续要对_jsonRoot进行销毁），检测是否解析成功
	cJSON* _jsonRoot = cJSON_Parse(_strStream.str().c_str());
	if (!_jsonRoot) return false;

	//类型检测
	if (_jsonRoot->type != cJSON_Object) return false;
	#pragma endregion

	//解析基本配置
	if (!ParseBasicConfigPrefab(basicPrefab, cJSON_GetObjectItem(_jsonRoot, "basic"))) return false;

	//解析玩家配置
	if (!ParsePlayerConfigPrefab(playerPrefab, cJSON_GetObjectItem(_jsonRoot, "player"))) return false;

	//解析各防御塔配置
	cJSON* _jsonTower = cJSON_GetObjectItem(_jsonRoot, "tower");
	if (!ParseTowerConfigPrefab(archerPrefab, cJSON_GetObjectItem(_jsonTower, "archer"))) return false;
	if (!ParseTowerConfigPrefab(axemanPrefab, cJSON_GetObjectItem(_jsonTower, "axeman"))) return false;
	if (!ParseTowerConfigPrefab(gunnerPrefab, cJSON_GetObjectItem(_jsonTower, "gunner"))) return false;

	//解析各敌人配置
	cJSON* _jsonEnemyType = cJSON_GetObjectItem(_jsonRoot, "enemy_type");
	if (!ParseEnemyConfigPrefab(slimePrefab, cJSON_GetObjectItem(_jsonEnemyType, "slime"))) return false;
	if (!ParseEnemyConfigPrefab(slimePrefab, cJSON_GetObjectItem(_jsonEnemyType, "king_slime"))) return false;
	if (!ParseEnemyConfigPrefab(slimePrefab, cJSON_GetObjectItem(_jsonEnemyType, "skeleton"))) return false;
	if (!ParseEnemyConfigPrefab(slimePrefab, cJSON_GetObjectItem(_jsonEnemyType, "goblin"))) return false;
	if (!ParseEnemyConfigPrefab(slimePrefab, cJSON_GetObjectItem(_jsonEnemyType, "priest_goblin"))) return false;

	//释放内存并返回
	cJSON_Delete(_jsonRoot);
	return true;
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

	//将_strStream的内容转化为C风格字符数组后解析并存储（cJSON_Parse方法是开辟到堆区的，故后续要对_jsonRoot进行销毁），检测是否解析成功
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
					else if (_jsonEnemyType->valuestring == "SlimeKing") { _spawnEvent.enemyType = EnemyType::SlimeKing; }
					else if (_jsonEnemyType->valuestring == "Skeleton") { _spawnEvent.enemyType = EnemyType::Skeleton; }
					else if (_jsonEnemyType->valuestring == "Goblin") { _spawnEvent.enemyType = EnemyType::Goblin; }
					else if (_jsonEnemyType->valuestring == "GoblinPriest") { _spawnEvent.enemyType = EnemyType::GoblinPriest; }
				}
			}
		}

		//如果上述三个值中的最后一个（生成事件列表）为空，则说明该_wave读取失败（前两者都有默认值，读取失败就采取默认值），需要被从waveList中弹出
		if (_wave.spawnEventList.empty())
			waveList.pop_back();
	}

	//防止内存泄漏；因为_jsonRoot是开辟在堆区的，而_jsonWave是对后者的部分拷贝（依赖关系，需注意销毁顺序），所以需要对二者进行销毁
	cJSON_Delete(_jsonWave);
	cJSON_Delete(_jsonRoot);

	//检测waveList是否为空，为空则说明配置文件编写有误
	if (waveList.empty()) return false;
	//一切都没毛病的话，就最终返回成功的信号
	return true;
}

bool ConfigManager::ParseBasicConfigPrefab(BasicConfigPrefab& _prefab, cJSON* _jsonRoot)
{
	//检测传入类型
	if (!_jsonRoot || _jsonRoot->type != cJSON_Object)
		return false;

	cJSON* _jsonWindowWidth = cJSON_GetObjectItem(_jsonRoot, "window_width");
	cJSON* _jsonWindowHeight = cJSON_GetObjectItem(_jsonRoot, "window_height");
	cJSON* _jsonWindowTitle = cJSON_GetObjectItem(_jsonRoot, "window_title");

	if (!_jsonWindowWidth || !_jsonWindowHeight || !_jsonWindowTitle
		|| _jsonWindowWidth->type != cJSON_Number
		|| _jsonWindowHeight->type != cJSON_Number
		|| _jsonWindowTitle->type != cJSON_String)
	{
		return false;
	}

	_prefab.windowWidth = _jsonWindowWidth->valueint;
	_prefab.windowHeight = _jsonWindowHeight->valueint;
	_prefab.windowTitle = _jsonWindowTitle->valuestring;

	return true;
}

bool ConfigManager::ParsePlayerConfigPrefab(PlayerConfigPrefab& _prefab, cJSON* _jsonRoot)
{
	//检测传入类型
	if (!_jsonRoot || _jsonRoot->type != cJSON_Object)
		return false;

	cJSON* _jsonSpeed = cJSON_GetObjectItem(_jsonRoot, "speed");
	if (!_jsonSpeed || _jsonSpeed->type != cJSON_Number) return false;
	_prefab.speed = _jsonSpeed->valuedouble;
	
	cJSON* _jsonNormalAtkCd = cJSON_GetObjectItem(_jsonRoot, "normal_attack_cooldown");
	if (!_jsonNormalAtkCd || _jsonNormalAtkCd->type != cJSON_Number) return false;
	_prefab.normalAttackCooldown = _jsonNormalAtkCd->valuedouble;
	
	cJSON* _jsonNormalAtkDmg = cJSON_GetObjectItem(_jsonRoot, "normal_attack_damage");
	if (!_jsonNormalAtkDmg || _jsonNormalAtkDmg->type != cJSON_Number) return false;
	_prefab.normalAttackDamage = _jsonNormalAtkDmg->valuedouble;
	
	cJSON* _jsonSkillCd = cJSON_GetObjectItem(_jsonRoot, "skill_cooldown");
	if (!_jsonSkillCd || _jsonSkillCd->type != cJSON_Number) return false;
	_prefab.skillCooldown = _jsonSkillCd->valuedouble;

	cJSON* _jsonSkillDmg = cJSON_GetObjectItem(_jsonRoot, "skill_damage");
	if (!_jsonSkillDmg || _jsonSkillDmg->type != cJSON_Number) return false;
	_prefab.skillDamage = _jsonSkillDmg->valuedouble;

	return true;
}

bool ConfigManager::ParseTowerConfigPrefab(TowerConfigPrefab& _prefab, cJSON* _jsonRoot)
{
	//检测传入类型
	if (!_jsonRoot || _jsonRoot->type != cJSON_Object)
		return false;

	cJSON* _jsonCooldown = cJSON_GetObjectItem(_jsonRoot, "cooldown");
	if (!ParseNumberArray(_prefab.cooldown, TOWER_MAX_LEVEL, _jsonCooldown)) return false;

	cJSON* _jsonDamage = cJSON_GetObjectItem(_jsonRoot, "damage");
	if (!ParseNumberArray(_prefab.damage, TOWER_MAX_LEVEL, _jsonDamage)) return false;
	
	cJSON* _jsonViewRange = cJSON_GetObjectItem(_jsonRoot, "view_range");
	if (!ParseNumberArray(_prefab.viewRange, TOWER_MAX_LEVEL, _jsonViewRange)) return false;
	
	cJSON* _jsonBuildCost = cJSON_GetObjectItem(_jsonRoot, "build_cost");
	if (!ParseNumberArray(_prefab.buildCost, TOWER_MAX_LEVEL, _jsonBuildCost)) return false;

	cJSON* _jsonUpgradeCost = cJSON_GetObjectItem(_jsonRoot, "upgrade_cost");
	//注意此处传入的长度是(TOWER_MAX_LEVEL - 1)，因为防御塔的初始等级是1，所以可升级次数是最高等级减1
	if (!ParseNumberArray(_prefab.upgradeCost, TOWER_MAX_LEVEL - 1, _jsonUpgradeCost)) return false;

	return true;
}

bool ConfigManager::ParseEnemyConfigPrefab(EnemyConfigPrefab& _prefab, cJSON* _jsonRoot)
{
	//检测传入类型
	if (!_jsonRoot || _jsonRoot->type != cJSON_Object)
		return false;

	cJSON* _jsonHP = cJSON_GetObjectItem(_jsonRoot, "hp");
	if (!_jsonHP || _jsonHP->type != cJSON_Number) return false;
	_prefab.hp = _jsonHP->valuedouble;

	cJSON* _jsonSpeed = cJSON_GetObjectItem(_jsonRoot, "speed");
	if (!_jsonSpeed || _jsonSpeed->type != cJSON_Number) return false;
	_prefab.speed = _jsonSpeed->valuedouble;

	cJSON* _jsonDamage = cJSON_GetObjectItem(_jsonRoot, "damage");
	if (!_jsonDamage || _jsonDamage->type != cJSON_Number) return false;
	_prefab.damage = _jsonDamage->valuedouble;

	cJSON* _jsonCoinRatio = cJSON_GetObjectItem(_jsonRoot, "coin_ratio");
	if (!_jsonCoinRatio || _jsonCoinRatio->type != cJSON_Number) return false;
	_prefab.coinRatio = _jsonCoinRatio->valuedouble;

	cJSON* _jsonRecoverCooldown = cJSON_GetObjectItem(_jsonRoot, "recover_cooldown");
	if (!_jsonRecoverCooldown || _jsonRecoverCooldown->type != cJSON_Number) return false;
	_prefab.recoverCooldown = _jsonRecoverCooldown->valuedouble;

	cJSON* _jsonRecoverRange = cJSON_GetObjectItem(_jsonRoot, "recover_range");
	if (!_jsonRecoverRange || _jsonRecoverRange->type != cJSON_Number) return false;
	_prefab.recoverRange = _jsonRecoverRange->valuedouble;

	cJSON* _jsonRecoveIntensity = cJSON_GetObjectItem(_jsonRoot, "recover_intensity");
	if (!_jsonRecoveIntensity || _jsonRecoveIntensity->type != cJSON_Number) return false;
	_prefab.recoverIntensity = _jsonRecoveIntensity->valuedouble;

	return true;
}

bool ConfigManager::ParseNumberArray(double* _arr, int _maxLength, cJSON* _jsonRoot)
{
	//检测是否为数组类型
	if (!_jsonRoot || _jsonRoot->type != cJSON_Array)
		return false;

	//用于记录循环的轮次，使得我们可以通过索引访问数组元素
	int _idx = -1;
	cJSON* _jsonElem = nullptr;
	cJSON_ArrayForEach(_jsonElem, _jsonRoot)
	{
		//递增索引
		_idx++;

		//若检测到错误类型，则说明配置文件有毛病
		if (_jsonElem->type != cJSON_Number)
			return false;

		//_maxLength的意义是，比如配置文件中的该数组存储了20个防御塔等级的数值，但是我只想要前10个
		if (_idx >= _maxLength)
			continue;

		//读取数据
		_arr[_idx] = _jsonElem->valuedouble;
	}

	return true;
}

#endif
