#include "../../../Header/Manager/Concrete/WaveManager.h"
#include "../../../Header/Manager/Concrete/ProcessManager.h"
#include "../../../Header/Manager/Concrete/ConfigManager.h"
#include "../../../Header/Manager/Concrete/EnemyManager.h"

WaveManager::WaveManager()
{
	//��ȡ�����б�
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
	//����Ϊ��������
	newWaveTimer.SetOneShot(false);
	//��ʼ���ȴ�ʱ��Ϊ��һ�����ε�interval
	newWaveTimer.SetWaitTime(waveList[0].interval);
	//���ò��ο�ʼ�Ļص�����
	newWaveTimer.SetTimeOutTrigger(
		//�����ò����ⲿ����
		[&]()
		{
			//�µĲ��ο������ͽ���bool��Ϊ�٣�����OnUpdate�в����²��μ�ʱ��
			isWaveEnded = false;

			//���Ų��ν���
			if (waveIdx >= 0 && waveIdx < waveList.size())
				_pm->IncreaseCoinNumBy(waveList[waveIdx].coinRewards);

			//������������������waveIdx�ĳ�ʼֵΪ-1�����״δ���ʱ������õ�0
			waveIdx++;
			//����ʣ�ನ�εĻ����Ǿ��������μ�ʱ����׼�����ڵ�ǰ�������������¼���ɺ󣩿�����һ����
			if (waveIdx < waveList.size())
			{
				std::cout << "Wave[" << waveIdx << "] Triggered\n";

				//���������¼�����Ϊ-1������ʼ���²��εĵ�һ��interval
				eventIdx = -1;
				newEventTimer.SetWaitTime(waveList[waveIdx].spawnEventList[0].interval);
				newEventTimer.Restart();

				//��ȡ�²�������ʱ������Ȼ������
				newWaveTimer.SetWaitTime(waveList[waveIdx].interval);
				newWaveTimer.Restart();
			}
			//��������Χ����˵�����в��ν�������Ϸ����
			else
			{
				_pm->isGameOver = true;
				//�����û�˾����ˣ�����Ĭ��ΪӮ
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
			//��ȡ��ǰ���ε������¼��б���ֹԽ��
			if (waveIdx >= waveList.size())
				return;
			const std::vector<Wave::SpawnEvent>& _eventList = waveList[waveIdx].spawnEventList;

			//��������¼���ɺ������������
			eventIdx++;
			//����Ƿ����µ������¼���Ҫִ��
			if (eventIdx < _eventList.size())
			{
				//���յ�ǰ�����¼��������ڶ�Ӧ������·�������ɶ�Ӧ����ĵ���
				EnemyManager::Instance()->SpawnEnemy(_eventList[eventIdx].enemyType, _eventList[eventIdx].spawnPoint);
				std::cout << "Event[" << eventIdx << "] Triggered\n";

				//��ȡ�µ����ɼ��ʱ�䣬Ȼ��������ʱ����ִ����һ�������¼�
				newEventTimer.SetWaitTime(_eventList[eventIdx].interval);
				newEventTimer.Restart();
			}
			else
			{
				//ִ�е��˴�˵����ǰ�����Ѿ�ִ�����
				isWaveEnded = true;

				//ֱ�ӷ��أ����������¼���ʱ���Ͳ�������
				return;
			}
		}
	);
	#pragma endregion
}

void WaveManager::OnUpdate(double _delta)
{	
	//��isWaveEnded�������²��μ�ʱ�����������¼���ʱ�������²��μ�ʱ������Ҫ���ϵ������
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