#ifndef _WAVE_MANAGER_HPP_
#define _WAVE_MANAGER_HPP_

#include "Manager.hpp"
#include "../Timer/Timer.hpp"
#include "ConfigManager.hpp"

//���ι�������ÿ�������ɶ�������¼�����
class WaveManager :Manager<WaveManager>
{
	friend Manager<WaveManager>;

private:
	int waveIdx = 0;                //��ǰ���ڽ��еĺ�۲�������
	int eventIdx = 0;               //��ǰ���ν��е�΢�������¼�������
	
	Timer waveStartTimer;           //���ڻ��Ѻ�۲��εļ�ʱ��
	Timer eventStartTimer;          //���ڻ���΢�������¼��ļ�ʱ��

	bool isWaveStarted = false;     //���ο�ʼ
	bool isAllWaveEnded = false;    //���в��ν������

private:
	WaveManager();
	~WaveManager() = default;
};

WaveManager::WaveManager()
{
	static const std::vector<Wave>& _waveList = ConfigManager::GetInstance()->waveList;

	//ÿ�������²��ε���ʱ�����øü�ʱ���Կ�������
	waveStartTimer.SetOneShot(true);
	//��ʼ���ȴ�ʱ��Ϊ��һ�����ε�interval
	waveStartTimer.SetWaitTime(_waveList[0].interval);
	//���ò��ο�ʼ�Ļص�����
	waveStartTimer.SetTimeOutTrigger(
		//�����ò����ⲿ����
		[&]()
		{
			//����ĳ�����Σ�ע�ⲻ�ǵ�ָ��һ�Σ�
			isWaveStarted = true;
			//���ο�ʼʱ�ŻῪʼ��Ӧ�ĸ��������¼�����ʼ���ȴ�ʱ��Ϊÿ�����εĵ�һ�������¼���interval
			eventStartTimer.SetWaitTime(_waveList[waveIdx].spawnEventList[0].interval);
		}
	);
}

#endif
