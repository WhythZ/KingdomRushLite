#ifndef _WAVE_HPP_
#define _WAVE_HPP_

#include <vector>
#include "EnemyType.hpp"

//存储怪物波次信息的结构体，一个波次由不小于一次的生成事件（以一定时间间隔）组成
struct Wave
{
	//单个怪物的生成事件
	struct SpawnEvent
	{
		//生成怪物的时间间隔
		double interval = 0;
		//生成位置的索引
		int spawnPoint = 1;
		//生成怪物的类型
		EnemyType enemyType = EnemyType::Slime;
	};

	double rewards = 0;			             //该波次结束后的奖励
	double interval = 0;			         //该波次的时间间隔
	std::vector<SpawnEvent> spawnEventList;  //该波次的生成事件列表
};

#endif