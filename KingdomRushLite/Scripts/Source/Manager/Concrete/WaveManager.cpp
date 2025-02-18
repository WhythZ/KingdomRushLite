#include "../../../Header/Manager/Concrete/WaveManager.hpp"
#include "../../../Header/Manager/Concrete/ProcessManager.hpp"
#include "../../../Header/Manager/Concrete/ConfigManager.hpp"
#include "../../../Header/Manager/Concrete/EnemyManager.hpp"

WaveManager::WaveManager()
{
	//获取波次列表
	waveList = ConfigManager::Instance()->waveList;

	//每次生成新波次敌人时，启用该计时器以开启波次
	waveStartTimer.SetOneShot(true);
	//初始化等待时间为第一个波次的interval
	waveStartTimer.SetWaitTime(waveList[0].interval);
	//设置波次开始的回调函数
	waveStartTimer.SetTimeOutTrigger(
		//以引用捕获外部变量
		[&]()
		{
			//开启某个波次（注意不是单指第一次）
			isWaveStarted = true;
			isWaveEnded = false;
			//每当新的波次开始时，才会开始对应的各个生成事件，初始化等待时间为当前波次的第一个生成事件的interval
			eventStartTimer.SetWaitTime(waveList[waveIdx].spawnEventList[0].interval);
			//每次触发波次计时器的回调函数时，将生成事件计时器归零重启，以触发第一个生成事件
			eventStartTimer.Restart();
		}
	);

	//将生成事件计时器设置成单次触发，因为我们要对当前波次内的生成事件是否全部执行完毕进行检测，若没完成就重启计时器即可
	eventStartTimer.SetOneShot(true);
	eventStartTimer.SetTimeOutTrigger(
		[&]()
		{
			//获取当前波次的生成事件列表，然后依据索引获取当前生成事件
			const std::vector<Wave::SpawnEvent> _eventList = waveList[waveIdx].spawnEventList;
			const Wave::SpawnEvent _event = _eventList[eventIdx];

			//按照当前生成事件的设置，在对应出生点路径上生成对应种类的敌人
			EnemyManager::Instance()->SpawnEnemy(_event.enemyType, _event.spawnPoint);

			//这次生成事件完成后，遂递增其索引
			eventIdx++;
			//检测是否还有新的生成事件需要执行，如果没有就无需重启计时器，反之则需要重启
			if (eventIdx >= _eventList.size())
			{
				//执行到此处说明当前波次已经执行完毕
				isWaveEnded = true;
				isWaveStarted = false;
				//直接返回即可，这样计时器就不会重启
				return;
			}
			//波次还没有执行完毕，由于此计时器是设置为单次触发的，故需重启计时器以执行下一个生成事件（重启前需要读取新的生成间隔时间）
			eventStartTimer.SetWaitTime(_eventList[eventIdx].interval);
			eventStartTimer.Restart();
		}
	);
}

void WaveManager::OnUpdate(double _delta)
{
	ProcessManager* _process = ProcessManager::Instance();

	//如果游戏结束，就不需更新了
	if (_process->isGameOver)
		return;

	//若当前波次开始了，那就更新生成事件计时器，否则进行波次计时器的更新
	if (!isWaveStarted)
		waveStartTimer.OnUpdate(_delta);
	else
		eventStartTimer.OnUpdate(_delta);

	//若波次结束，不能立刻重置波次计时器生成下一波，得等场上敌人都清空了再开始
	if (isWaveEnded && EnemyManager::Instance()->IsEnemyCleaned())
	{
		//发放波次奖励
		ProcessManager::Instance()->IncreaseCoinNumBy(waveList[waveIdx].rewards);

		//递增波次索引（注意先发奖励再递增，否则会奖励错）
		waveIdx++;
		//若超出范围，则说明所有波次结束，游戏结束
		if (waveIdx >= waveList.size())
		{
			_process->isGameOver = true;
			//如果家没了就输了，否则默认为赢
			if (_process->GetCurrentHealth() <= 0)
				_process->isWin = false;
		}
		//还有剩余波次的话，那就重启波次计时器，准备开启下一波次
		else
		{
			//先重置生成事件索引
			eventIdx = 0;
			//然后获取新波器启动时间间隔，然后重启
			waveStartTimer.SetWaitTime(waveList[waveIdx].interval);
			waveStartTimer.Restart();
		}
	}
}
