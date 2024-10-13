#ifndef _WAVE_MANAGER_HPP_
#define _WAVE_MANAGER_HPP_

#include "Manager.hpp"
#include "../Timer/Timer.hpp"
#include "ConfigManager.hpp"

//波次管理器，每个波次由多个生成事件构成
class WaveManager :Manager<WaveManager>
{
	friend Manager<WaveManager>;

private:
	int waveIdx = 0;                //当前正在进行的宏观波次索引
	int eventIdx = 0;               //当前波次进行的微观生成事件的索引
	
	Timer waveStartTimer;           //用于唤醒宏观波次的计时器
	Timer eventStartTimer;          //用于唤醒微观生成事件的计时器

	bool isWaveStarted = false;     //波次开始
	bool isAllWaveEnded = false;    //所有波次进行完毕

private:
	WaveManager();
	~WaveManager() = default;
};

WaveManager::WaveManager()
{
	static const std::vector<Wave>& _waveList = ConfigManager::GetInstance()->waveList;

	//每次生成新波次敌人时，启用该计时器以开启波次
	waveStartTimer.SetOneShot(true);
	//初始化等待时间为第一个波次的interval
	waveStartTimer.SetWaitTime(_waveList[0].interval);
	//设置波次开始的回调函数
	waveStartTimer.SetTimeOutTrigger(
		//以引用捕获外部变量
		[&]()
		{
			//开启某个波次（注意不是单指第一次）
			isWaveStarted = true;
			//波次开始时才会开始对应的各个生成事件，初始化等待时间为每个波次的第一个生成事件的interval
			eventStartTimer.SetWaitTime(_waveList[waveIdx].spawnEventList[0].interval);
		}
	);
}

#endif
