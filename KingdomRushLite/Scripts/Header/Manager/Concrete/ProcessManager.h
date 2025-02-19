#ifndef _PROCESS_MANAGER_H_
#define _PROCESS_MANAGER_H_

#include "../Manager.hpp"
#include "../../Map/Map.h"

//��¼һ����Ϸ�еĸ��̬����
class ProcessManager :public Manager<ProcessManager>
{
	friend Manager<ProcessManager>;

public:
	bool isGameOver = false;             //����ȫ������������Ϸ����
	bool isWin = true;                   //��û�����ƣ���Ϸ��û����

	#pragma region Map
	Map map;                             //��Ϸ��ͼ
	SDL_Rect mapRect = { 0 };            //��ͼ������Ⱦ����Ϸ�����ڵ�λ���볤��
	#pragma endregion

	#pragma region Tower
	int levelArcher = 0;                 //�������ȼ���ȫ��ͳһ������
	int levelAxeman = 0;                 //Ͷ���ֵȼ�
	int levelGunner = 0;                 //ǹ���ֵȼ�
	#pragma endregion

private:
	#pragma region Home
	double healthMaximum;                //��¼�ҵ����Ѫ��
	double healthCurrent;                //��¼�ҵĵ�ǰѪ��
	#pragma endregion

	#pragma region Coin
	double coinNum = 0;                  //��ҳ��еĽ������
	#pragma endregion

public:
	void DecreaseHealthBy(double);       //���ٵ�ǰ��Ѫ��
	void IncreaseCoinNumBy(double);      //���ӽ����
	void DecreaseCoinNumBy(double);      //���ٽ����

	double GetCurrentHealth() const;     //��ȡ��ǰ��Ѫ��
	double GetCurrentCoin() const;       //��ȡ��ǰ�����

private:
	ProcessManager();
	~ProcessManager() = default;
};

#endif