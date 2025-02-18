#ifndef _WAVE_MANAGER_H_
#define _WAVE_MANAGER_H_

#include <vector>
#include "../Manager.hpp"
#include "../../Math/Timer.h"
#include "../../Enemy/Wave.h"

//波次管理器，每个波次由多个生成事件构成
class WaveManager :public Manager<WaveManager>
{
	friend Manager<WaveManager>;

private:
	std::vector<Wave> waveList;     //记录波次列表
	int waveIdx = 0;                //当前正在进行的宏观波次索引
	int eventIdx = 0;               //当前波次进行的微观生成事件的索引
	
	Timer waveStartTimer;           //用于唤醒宏观波次的计时器
	Timer eventStartTimer;          //用于唤醒微观生成事件的计时器

	bool isWaveStarted = false;     //波次开始
	bool isWaveEnded = false;       //当前波次的所有生成事件执行完毕

public:
	void OnUpdate(double);          //帧更新函数

private:
	WaveManager();
	~WaveManager() = default;
};

#endif
