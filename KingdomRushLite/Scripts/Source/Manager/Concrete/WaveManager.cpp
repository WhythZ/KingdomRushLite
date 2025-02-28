#include "../../../Header/Manager/Concrete/WaveManager.h"
#include "../../../Header/Manager/Concrete/ProcessManager.h"
#include "../../../Header/Manager/Concrete/ConfigManager.h"
#include "../../../Header/Manager/Concrete/EnemyManager.h"

WaveManager::WaveManager()
{
	//��ȡ�����б�
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

			//��������ɲ��εĽ������ȷ������ٵ�������������ά����
			if (waveIdx != -1)
				_pm->IncreaseCoinNumBy(waveList[waveIdx].coinRewards);

			//������������������waveIdx�ĳ�ʼֵΪ-1�����״δ���ʱ������õ�0
			waveIdx++;
			//std::cout << "CurrentWaveIdx=" << waveIdx << "\n";
			//��������Χ����˵�����в��ν�������Ϸ����
			if (waveIdx >= waveList.size())
			{
				_pm->isGameOver = true;
				//�����û�˾����ˣ�����Ĭ��ΪӮ
				if (_pm->GetCurrentHealth() <= 0)
					_pm->isWin = false;
			}
			//����ʣ�ನ�εĻ����Ǿ��������μ�ʱ����׼�����ڵ�ǰ�������������¼���ɺ󣩿�����һ����
			else
			{
				//���������¼�����Ϊ0������ʼ���²��εĵ�һ��interval
				eventIdx = 0;
				newEventTimer.SetWaitTime(waveList[waveIdx].spawnEventList[0].interval);
				newEventTimer.Restart();

				//��ȡ�²�������ʱ������Ȼ������
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
			//��ȡ��ǰ���ε������¼��б�Ȼ������������ȡ��ǰ�����¼�
			const std::vector<Wave::SpawnEvent> _eventList = waveList[waveIdx].spawnEventList;
			const Wave::SpawnEvent _event = _eventList[eventIdx];

			//���յ�ǰ�����¼������ã��ڶ�Ӧ������·�������ɶ�Ӧ����ĵ���
			EnemyManager::Instance()->SpawnEnemy(_event.enemyType, _event.spawnPoint);

			//��������¼���ɺ������������
			eventIdx++;
			//std::cout << "CurrentEventIdx=" << eventIdx << "\n";
			//����Ƿ����µ������¼���Ҫִ�У����û�о�����������ʱ������֮����Ҫ����
			if (eventIdx >= _eventList.size())
			{
				//ִ�е��˴�˵����ǰ�����Ѿ�ִ�����
				isWaveEnded = true;
				//ֱ�ӷ��ؼ��ɣ��������������¼���ʱ���Ͳ�������
				return;
			}
			//�����λ�û��ִ����ϣ����ڴ˼�ʱ��������Ϊ���δ����ģ�����������ʱ����ִ����һ�������¼�������ǰ��Ҫ��ȡ�µ����ɼ��ʱ�䣩
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
	//��isWaveEnded�������²��μ�ʱ�����������¼���ʱ��
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