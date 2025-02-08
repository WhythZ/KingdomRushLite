#ifndef _WAVE_HPP_
#define _WAVE_HPP_

#include <vector>
#include "EnemyType.hpp"

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

	double rewards = 0;			             //�ò��ν�����Ľ���
	double interval = 0;			         //�ò��ε�ʱ����
	std::vector<SpawnEvent> spawnEventList;  //�ò��ε������¼��б�
};

#endif