#include "../../../Header/Manager/Concrete/WaveManager.h"
#include "../../../Header/Manager/Concrete/ProcessManager.h"
#include "../../../Header/Manager/Concrete/ConfigManager.h"
#include "../../../Header/Manager/Concrete/EnemyManager.h"

WaveManager::WaveManager()
{
	//��ȡ�����б�
	waveList = ConfigManager::Instance()->waveList;
	//std::cout << "Wave[0]EventList.size()=" << waveList[0].spawnEventList.size() << "\n";
	//std::cout << "Wave[1]EventList.size()=" << waveList[1].spawnEventList.size() << "\n";
	//std::cout << "Wave[2]EventList.size()=" << waveList[2].spawnEventList.size() << "\n";
	//std::cout << "Wave[3]EventList.size()=" << waveList[3].spawnEventList.size() << "\n";
	//std::cout << "Wave[0]EventList[0].enemyType=" << waveList[0].spawnEventList[0].enemyType << "\n";

	#pragma region BigWaveTimer
	//ÿ�������²��ε���ʱ�����øü�ʱ���Կ�������
	waveStartTimer.SetOneShot(true);
	//��ʼ���ȴ�ʱ��Ϊ��һ�����ε�interval
	waveStartTimer.SetWaitTime(waveList[0].interval);
	//���ò��ο�ʼ�Ļص�����
	waveStartTimer.SetTimeOutTrigger(
		//�����ò����ⲿ����
		[&]()
		{
			//����ĳ�����Σ�ע�ⲻ�ǵ�ָ��һ�Σ�
			isWaveStarted = true;
			//ÿ���µĲ��ο�ʼʱ���ŻῪʼ��Ӧ�ĸ��������¼�����ʼ���ȴ�ʱ��Ϊ��ǰ���εĵ�һ�������¼���interval
			eventStartTimer.SetWaitTime(waveList[waveIdx].spawnEventList[0].interval);
			//ÿ�δ������μ�ʱ���Ļص�����ʱ���������¼���ʱ�������������Դ�����һ�������¼�
			eventStartTimer.Restart();
		}
	);
	#pragma endregion

	#pragma region SmallEventTimer
	//�������¼���ʱ�����óɵ��δ�������Ϊ����Ҫ�Ե�ǰ�����ڵ������¼��Ƿ�ȫ��ִ����Ͻ��м�⣬��û��ɾ�������ʱ������
	eventStartTimer.SetOneShot(true);
	eventStartTimer.SetTimeOutTrigger(
		[&]()
		{
			//��ȡ��ǰ���ε������¼��б�Ȼ������������ȡ��ǰ�����¼�
			const std::vector<Wave::SpawnEvent> _eventList = waveList[waveIdx].spawnEventList;
			const Wave::SpawnEvent _event = _eventList[eventIdx];

			//���յ�ǰ�����¼������ã��ڶ�Ӧ������·�������ɶ�Ӧ����ĵ���
			EnemyManager::Instance()->SpawnEnemy(_event.enemyType, _event.spawnPoint);

			//��������¼���ɺ������������
			eventIdx++;
			//����Ƿ����µ������¼���Ҫִ�У����û�о�����������ʱ������֮����Ҫ����
			if (eventIdx >= _eventList.size())
			{
				//ִ�е��˴�˵����ǰ�����Ѿ�ִ�����
				isWaveEnded = true;
				isWaveStarted = false;
				//ֱ�ӷ��ؼ��ɣ�������ʱ���Ͳ�������
				return;
			}
			//���λ�û��ִ����ϣ����ڴ˼�ʱ��������Ϊ���δ����ģ�����������ʱ����ִ����һ�������¼�������ǰ��Ҫ��ȡ�µ����ɼ��ʱ�䣩
			eventStartTimer.SetWaitTime(_eventList[eventIdx].interval);
			eventStartTimer.Restart();
		}
	);
	#pragma endregion
}

void WaveManager::OnUpdate(double _delta)
{	
	ProcessManager* _process = ProcessManager::Instance();

	//����ǰ���ο�ʼ�ˣ��Ǿ͸��������¼���ʱ����������в��μ�ʱ���ĸ���
	if (!isWaveStarted)
		waveStartTimer.OnUpdate(_delta);
	else
		eventStartTimer.OnUpdate(_delta);

	//�����ν������ҳ��ϵ�������գ������ò��μ�ʱ����������һ��
	if (isWaveEnded && EnemyManager::Instance()->IsEnemyCleaned())
	{
		//��ֹ��waveStartTimer�����ص�����ǰ�ظ�ִ�е�ǰ����������ݣ����²��ϵ�����������
		isWaveEnded = false;

		//���Ų��ν���
		ProcessManager::Instance()->IncreaseCoinNumBy(waveList[waveIdx].rewards);

		//��������������ע���ȷ������ٵ���������ά����
		waveIdx++;
		std::cout << "CurrentWaveIdx=" << waveIdx << "\n";
		//��������Χ����˵�����в��ν�������Ϸ����
		if (waveIdx >= waveList.size())
		{
			_process->isGameOver = true;
			//�����û�˾����ˣ�����Ĭ��ΪӮ
			if (_process->GetCurrentHealth() <= 0)
				_process->isWin = false;
		}
		//����ʣ�ನ�εĻ����Ǿ��������μ�ʱ����׼��������һ����
		else
		{
			//�����������¼�����
			eventIdx = 0;
			//Ȼ���ȡ�²�������ʱ������Ȼ������
			waveStartTimer.SetWaitTime(waveList[waveIdx].interval);
			waveStartTimer.Restart();
		}
	}
}
