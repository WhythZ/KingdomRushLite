#ifndef _PROCESS_MANAGER_HPP_
#define _PROCESS_MANAGER_HPP_

#include "../Manager.hpp"
#include "ConfigManager.hpp"
#include "AudioManager.hpp"
#include "../../Map/Map.hpp"

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

	#pragma region Coin
	double coinNum = 0;                  //��ҳ��еĽ������
	#pragma endregion

private:
	#pragma region Home
	double healthMaximum;                //��¼�ҵ����Ѫ��
	double healthCurrent;                //��¼�ҵĵ�ǰѪ��
	#pragma endregion

public:
	void IncreaseCoinNumBy(double);      //���ӽ����
	void DecreaseCoinNumBy(double);      //���ٽ����

	double GetCurrentHealth() const;     //��ȡ��ǰ��Ѫ��
	void DecreaseHealthBy(double);       //���ٵ�ǰ��Ѫ��

private:
	ProcessManager();
	~ProcessManager() = default;
};

ProcessManager::ProcessManager()
{
	//Ѫ����ȡ
	healthMaximum = ConfigManager::Instance()->initHomeHP;
	healthCurrent = healthMaximum;
}

double ProcessManager::GetCurrentHealth() const
{
	return healthCurrent;
}

void ProcessManager::DecreaseHealthBy(double _decre)
{
	//����Ѫ��
	healthCurrent -= _decre;
	if (healthCurrent <= 0)
		healthCurrent = 0;

	//�����ܻ���Ч
	AudioManager::Instance()->PlaySFX(SoundResID::HomeHurt);
}

void ProcessManager::IncreaseCoinNumBy(double _reward)
{
	coinNum += _reward;
}

void ProcessManager::DecreaseCoinNumBy(double _cost)
{
	coinNum = (coinNum - _cost > 0) ? (coinNum - _cost) : 0;
}

#endif