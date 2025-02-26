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
		std::string windowTitle = "KingdomRushLite";
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
		double fireRadius[10] = { 5 };
		//建造花费
		double buildCost[10] = { 50 };
		//升级花费
		double upgradeCost[10] = { 75 };
	};

	//敌人预制体
	struct EnemyConfigPrefab
	{
		//敌人血量
		double health = 100;
		//敌人移动速度
		double speed = 1;
		//敌人造成的伤害
		double damage = 1;
		//敌人爆金币的概率
		double coinRatio = 0.5;
		//敌人恢复技能的冷却、范围半径（-1为不恢复，0为只恢复自己）与恢复强度
		double skillRecoverCooldown = 10;
		double skillRecoverRadius = -1;
		double skillRecoverIntensity = 25;
	};

public:
	#pragma region LoadedSettings
	BasicConfigPrefab basicPrefab;          //存储基本配置信息
	PlayerConfigPrefab playerDragonPrefab;  //存储玩家配置信息

	std::vector<Wave> waveList;             //怪物波次列表

	TowerConfigPrefab archerPrefab;         //存储弓箭手配置信息
	TowerConfigPrefab axemanPrefab;         //存储投斧手配置信息
	TowerConfigPrefab gunnerPrefab;         //存储枪炮手配置信息

	EnemyConfigPrefab slimePrefab;          //存储史莱姆配置信息
	EnemyConfigPrefab slimeKingPrefab;      //存储史莱姆王配置信息
	EnemyConfigPrefab skeletonPrefab;       //存储骷髅配置信息
	EnemyConfigPrefab goblinPrefab;         //存储哥布林配置信息
	EnemyConfigPrefab goblinPriestPrefab;   //存储哥布林祭司配置信息
	#pragma endregion

	const int initCoinNum = 100;            //初始的金币数量常量
	const int pickCoinNum = 10;             //每拾取一个金币增加的金币数量常量

	const double initHomeHP = 10;           //家的初始默认血量常量

public:
	bool LoadConfig(const std::string&);    //加载游戏的各项基本设置（json）
	bool LoadWaves(const std::string&);     //加载关卡波次的具体信息（json）

private:
	ConfigManager() = default;
	~ConfigManager() = default;

	bool ParseBasicConfigPrefab(BasicConfigPrefab&, cJSON*);    //专门解析基本配置信息
	bool ParsePlayerConfigPrefab(PlayerConfigPrefab&, cJSON*);  //专门解析玩家配置信息
	bool ParseTowerConfigPrefab(TowerConfigPrefab&, cJSON*);    //专门解析防御塔配置信息
	bool ParseEnemyConfigPrefab(EnemyConfigPrefab&, cJSON*);    //专门解析敌人配置信息

	bool ParseNumberArray(double*, int, cJSON*);                //解析数字数组，如防御塔的各级属性
};

#endif
