#ifndef _CONFIG_MANAGER_HPP_
#define _CONFIG_MANAGER_HPP_

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cJSON.h>
#include <SDL.h>
#include "Manager.hpp"

//用于处理游戏配置文件config.json内的预制体信息
class ConfigManager : public Manager<ConfigManager>
{
	friend class Manager<ConfigManager>;

public:
	//游戏的基础配置信息预制体
	struct BasicConfigInfoPrefab
	{
		//窗口标题
		std::string windowTitle = "KingdomRush-Lite";
		//窗口高度与宽度
		int windowWidth = 1280;
		int windowHeight = 720;
	};

	//玩家预制体
	struct PlayerPrefab
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
	struct TowerPrefab
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
	struct EnemyPrefab
	{
		//敌人血量
		double hp = 100;
		//敌人移动速度
		double speed = 1;
		//敌人造成的伤害
		double damage = 1;
		//敌人爆金币的概率
		double coinRation = 0.5;
		//敌人恢复技能的冷却、范围（-1为不恢复，0为只恢复自己）与恢复强度
		double recoverCooldown = 10;
		double recoverRange = -1;
		double recoverIntensity = 25;
	};

protected:
	ConfigManager() = default;

	~ConfigManager() = default;
};

#endif
