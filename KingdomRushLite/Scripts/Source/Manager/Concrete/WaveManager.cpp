#include "../../../Header/Manager/Concrete/WaveManager.h"
#include "../../../Header/Manager/Concrete/ProcessManager.h"
#include "../../../Header/Manager/Concrete/ConfigManager.h"
#include "../../../Header/Manager/Concrete/EnemyManager.h"

WaveManager::WaveManager()
{
	//获取波次列表
	waveList = ConfigManager::Instance()->waveList;

	#pragma region DebugLog
	std::cout << "--------------------\nLoaded Waves\n";
	for (int _i = 0; _i < waveList.size(); _i++)
	{
		std::cout << "Wave[" << _i << "]: ";
		for (int _j = 0; _j < waveList[_i].spawnEventList.size(); _j++)
			std::cout << waveList[_i].spawnEventList[_j].enemyType << " ";
		std::cout << "\n";
	}
	std::cout << "--------------------\n";
	#pragma endregion

	static ProcessManager* _pm = ProcessManager::Instance();

	#pragma region SetWaveStartTimer
	//设置为连续触发
	newWaveTimer.SetOneShot(false);
	//初始化等待时间为第一个波次的interval
	newWaveTimer.SetWaitTime(waveList[0].interval);
	//设置波次开始的回调函数
	newWaveTimer.SetTimeOutTrigger(
		//以引用捕获外部变量
		[&]()
		{
			//新的波次开启，就将此bool设为假，以让OnUpdate中不更新波次计时器
			isWaveEnded = false;

			//发放波次奖励
			if (waveIdx >= 0 && waveIdx < waveList.size())
				_pm->IncreaseCoinNumBy(waveList[waveIdx].coinRewards);

			//递增波次索引，由于waveIdx的初始值为-1，故首次触发时递增会得到0
			waveIdx++;
			//还有剩余波次的话，那就重启波次计时器，准备（在当前波次所有生成事件完成后）开启下一波次
			if (waveIdx < waveList.size())
			{
				std::cout << "Wave[" << waveIdx << "] Triggered\n";

				//重置生成事件索引为-1，并初始化新波次的第一个interval
				eventIdx = -1;
				newEventTimer.SetWaitTime(waveList[waveIdx].spawnEventList[0].interval);
				newEventTimer.Restart();

				//获取新波次启动时间间隔，然后重启
				newWaveTimer.SetWaitTime(waveList[waveIdx].interval);
				newWaveTimer.Restart();
			}
			//若超出范围，则说明所有波次结束，游戏结束
			else
			{
				_pm->isGameOver = true;
				//如果家没了就输了，否则默认为赢
				if (_pm->GetCurrentHealth() <= 0)
					_pm->isWin = false;
			}
		}
	);
	#pragma endregion

	#pragma region SetEventStartTimer
	newEventTimer.SetOneShot(false);
	newEventTimer.SetTimeOutTrigger(
		[&]()
		{
			//获取当前波次的生成事件列表，防止越界
			if (waveIdx >= waveList.size())
				return;
			const std::vector<Wave::SpawnEvent>& _eventList = waveList[waveIdx].spawnEventList;

			//这次生成事件完成后，遂递增其索引
			eventIdx++;
			//检测是否还有新的生成事件需要执行
			if (eventIdx < _eventList.size())
			{
				//依照当前生成事件的设置在对应出生点路径上生成对应种类的敌人
				EnemyManager::Instance()->SpawnEnemy(_eventList[eventIdx].enemyType, _eventList[eventIdx].spawnPoint);
				std::cout << "Event[" << eventIdx << "] Triggered\n";

				//读取新的生成间隔时间，然后重启计时器以执行下一个生成事件
				newEventTimer.SetWaitTime(_eventList[eventIdx].interval);
				newEventTimer.Restart();
			}
			else
			{
				//执行到此处说明当前波次已经执行完毕
				isWaveEnded = true;

				//直接返回，怪物生成事件计时器就不会重启
				return;
			}
		}
	);
	#pragma endregion
}

void WaveManager::OnUpdate(double _delta)
{	
	//由isWaveEnded决定更新波次计时器还是生成事件计时器，更新波次计时器还需要场上敌人清空
	static EnemyManager* _em = EnemyManager::Instance();
	if (isWaveEnded && _em->IsEnemyCleaned())
		newWaveTimer.OnUpdate(_delta);
	if (!isWaveEnded)
		newEventTimer.OnUpdate(_delta);
}

size_t WaveManager::GetCurrentWaveIdx() const
{
	return waveIdx;
}

size_t WaveManager::GetTotalWaveNum() const
{
	return waveList.size();
}