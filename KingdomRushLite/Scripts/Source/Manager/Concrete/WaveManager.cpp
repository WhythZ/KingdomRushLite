#include "../../../Header/Manager/Concrete/WaveManager.h"
#include "../../../Header/Manager/Concrete/ProcessManager.h"
#include "../../../Header/Manager/Concrete/ConfigManager.h"
#include "../../../Header/Manager/Concrete/EnemyManager.h"

WaveManager::WaveManager()
{
	//获取波次列表
	waveList = ConfigManager::Instance()->waveList;
	//for (int _i = 0; _i < waveList.size(); _i++)
	//{
	//	std::cout << "Wave[" << _i << "]: ";
	//	for (int _j = 0; _j < waveList[_i].spawnEventList.size(); _j++)
	//		std::cout << waveList[_i].spawnEventList[_j].enemyType << " ";
	//	std::cout << "\n";
	//}

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

			//发放已完成波次的奖励（先发奖励再递增索引，否则会奖励错）
			if (waveIdx != -1)
				_pm->IncreaseCoinNumBy(waveList[waveIdx].coinRewards);

			//递增波次索引，由于waveIdx的初始值为-1，故首次触发时递增会得到0
			waveIdx++;
			//std::cout << "CurrentWaveIdx=" << waveIdx << "\n";
			//若超出范围，则说明所有波次结束，游戏结束
			if (waveIdx >= waveList.size())
			{
				_pm->isGameOver = true;
				//如果家没了就输了，否则默认为赢
				if (_pm->GetCurrentHealth() <= 0)
					_pm->isWin = false;
			}
			//还有剩余波次的话，那就重启波次计时器，准备（在当前波次所有生成事件完成后）开启下一波次
			else
			{
				//重置生成事件索引为0，并初始化新波次的第一个interval
				eventIdx = 0;
				newEventTimer.SetWaitTime(waveList[waveIdx].spawnEventList[0].interval);
				newEventTimer.Restart();

				//获取新波次启动时间间隔，然后重启
				newWaveTimer.SetWaitTime(waveList[waveIdx].interval);
				newWaveTimer.Restart();
			}
		}
	);
	#pragma endregion

	#pragma region SetEventStartTimer
	newEventTimer.SetOneShot(false);
	newEventTimer.SetTimeOutTrigger(
		[&]()
		{
			//获取当前波次的生成事件列表，然后依据索引获取当前生成事件
			const std::vector<Wave::SpawnEvent> _eventList = waveList[waveIdx].spawnEventList;
			const Wave::SpawnEvent _event = _eventList[eventIdx];

			//按照当前生成事件的设置，在对应出生点路径上生成对应种类的敌人
			EnemyManager::Instance()->SpawnEnemy(_event.enemyType, _event.spawnPoint);

			//这次生成事件完成后，遂递增其索引
			eventIdx++;
			//std::cout << "CurrentEventIdx=" << eventIdx << "\n";
			//检测是否还有新的生成事件需要执行，如果没有就无需重启计时器，反之则需要重启
			if (eventIdx >= _eventList.size())
			{
				//执行到此处说明当前波次已经执行完毕
				isWaveEnded = true;
				//直接返回即可，这样怪物生成事件计时器就不会重启
				return;
			}
			//若波次还没有执行完毕，由于此计时器是设置为单次触发的，故需重启计时器以执行下一个生成事件（重启前需要读取新的生成间隔时间）
			else
			{
				newEventTimer.SetWaitTime(_eventList[eventIdx].interval);
				newEventTimer.Restart();
			}
		}
	);
	#pragma endregion
}

void WaveManager::OnUpdate(double _delta)
{	
	//由isWaveEnded决定更新波次计时器还是生成事件计时器
	if (isWaveEnded)
		newWaveTimer.OnUpdate(_delta);
	else
		newEventTimer.OnUpdate(_delta);
}

int WaveManager::GetCurrentWaveIdx() const
{
	return waveIdx;
}

int WaveManager::GetTotalWaveNum() const
{
	return waveList.size();
}