#ifndef _PROCESS_MANAGER_H_
#define _PROCESS_MANAGER_H_

#include "../Manager.hpp"
#include "../../Map/Map.h"
#include "../../Tower/TowerType.h"

//���������ļ����ø���������ĵȼ�����
#define TOWER_ARCHER_MAX_LEVEL 9
#define TOWER_AXEMAN_MAX_LEVEL 9
#define TOWER_GUNNER_MAX_LEVEL 9

//��¼һ����Ϸ�еĸ��̬����
class ProcessManager :public Manager<ProcessManager>
{
	friend class Manager<ProcessManager>;

public:
	bool isGameOver = false;                   //����ȫ������������Ϸ����
	bool isWin = true;                         //��û�����ƣ���Ϸ��û����

	#pragma region Map
	Map map;                                   //��Ϸ��ͼ
	SDL_Rect mapRect = { 0 };                  //��ͼ������Ⱦ����Ϸ�����ڵ�λ���볤��
	#pragma endregion

private:
	#pragma region Home
	double healthMaximum;                      //��¼�ҵ����Ѫ��
	double healthCurrent;                      //��¼�ҵĵ�ǰѪ��
	#pragma endregion

	#pragma region Coin
	double coinNum = 1000;                        //��ҳ��еĽ������
	#pragma endregion

	#pragma region Tower
	int levelArcher = 0;                       //�������ȼ���ȫ��ͳһ������
	int levelAxeman = 0;                       //Ͷ���ֵȼ�
	int levelGunner = 0;                       //ǹ���ֵȼ�
	#pragma endregion

public:
	double GetCurrentHealth() const;           //��ȡ��ǰ��Ѫ��
	void DecreaseHealthBy(double);             //���ٵ�ǰ��Ѫ��

	double GetCurrentCoinNum() const;          //��ȡ��ǰ�����
	void IncreaseCoinNumBy(double);            //���ӽ����
	void DecreaseCoinNumBy(double);            //���ٽ����

	int GetTowerLevel(TowerType) const;        //��ȡ��Ӧ�������ȼ�
	void IncreaseTowerLevelBy(TowerType, int); //���Ӷ�Ӧ�������ȼ�

private:
	ProcessManager();
	~ProcessManager() = default;
};

#endif