#ifndef _PROCESS_MANAGER_HPP_
#define _PROCESS_MANAGER_HPP_

#include "../Manager.hpp"

//��¼һ����Ϸ�еĸ��̬����
class ProcessManager :public Manager<ProcessManager>
{
	friend Manager<ProcessManager>;

public:
	bool isGameOver = false;             //����ȫ������������Ϸ����
	bool isWin = true;                   //��û�����ƣ���Ϸ��û����

	#pragma region TowerLevel
	int levelArcher = 0;                 //�������ȼ���ȫ��ͳһ������
	int levelAxeman = 0;                 //Ͷ���ֵȼ�
	int levelGunner = 0;                 //ǹ���ֵȼ�
	#pragma endregion

private:
	ProcessManager() = default;
	~ProcessManager() = default;
};

#endif