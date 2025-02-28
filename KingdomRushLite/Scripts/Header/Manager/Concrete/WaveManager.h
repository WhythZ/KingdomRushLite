#ifndef _WAVE_MANAGER_H_
#define _WAVE_MANAGER_H_

#include <vector>
#include "../Manager.hpp"
#include "../../Infra/Timer.h"
#include "../../Enemy/Wave.h"

//波次管理器，每个波次由多个生成事件构成
class WaveManager :public Manager<WaveManager>
{
	friend class Manager<WaveManager>;

private:
	std::vector<Wave> waveList;     //记录波次列表
	int waveIdx = -1;               //当前正在进行的宏观波次索引
	int eventIdx = 0;               //当前波次进行的微观生成事件的索引
	
	Timer newWaveTimer;             //用于唤起宏观波次的计时器
	Timer newEventTimer;            //用于唤起微观生成事件的计时器

	bool isWaveEnded = true;        //当前波次的所有生成事件执行完毕

public:
	void OnUpdate(double);          //帧更新函数

	int GetCurrentWaveIdx() const;  //获取当前进行到的波次数
	int GetTotalWaveNum() const;    //获取总波次数

private:
	WaveManager();
	~WaveManager() = default;
};

#endif
