#ifndef _WAVE_H_
#define _WAVE_H_

#include <vector>
#include "EnemyType.h"

//�洢���ﲨ����Ϣ�Ľṹ�壬һ�������ɲ�С��һ�ε������¼�����һ��ʱ���������
struct Wave
{
	//��������������¼�
	struct SpawnEvent
	{
		//���ɹ����ʱ����
		double interval = 0;
		//����λ�õ�����
		int spawnPoint = 1;
		//���ɹ��������
		EnemyType enemyType = EnemyType::Slime;
	};

	double coinRewards = 0;			         //�ò��ν�����Ľ�ҽ���
	double interval = 0;			         //�ò��ε�ʱ����
	std::vector<SpawnEvent> spawnEventList;  //�ò��ε������¼��б�
};

#endif