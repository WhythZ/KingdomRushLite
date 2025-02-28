#ifndef _WAVE_MANAGER_H_
#define _WAVE_MANAGER_H_

#include <vector>
#include "../Manager.hpp"
#include "../../Infra/Timer.h"
#include "../../Enemy/Wave.h"

//���ι�������ÿ�������ɶ�������¼�����
class WaveManager :public Manager<WaveManager>
{
	friend class Manager<WaveManager>;

private:
	std::vector<Wave> waveList;     //��¼�����б�
	int waveIdx = 0;                //��ǰ���ڽ��еĺ�۲�������
	int eventIdx = 0;               //��ǰ���ν��е�΢�������¼�������
	
	Timer waveStartTimer;           //���ڻ��Ѻ�۲��εļ�ʱ��
	Timer eventStartTimer;          //���ڻ���΢�������¼��ļ�ʱ��

	bool isWaveStarted = false;     //���ο�ʼ
	bool isWaveEnded = false;       //��ǰ���ε����������¼�ִ�����

public:
	void OnUpdate(double);          //֡���º���

	int GetCurrentWaveIdx() const;  //��ȡ��ǰ���е��Ĳ�����
	int GetTotalWaveNum() const;    //��ȡ�ܲ�����

private:
	WaveManager();
	~WaveManager() = default;
};

#endif
