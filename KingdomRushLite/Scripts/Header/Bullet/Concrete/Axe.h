#ifndef _AXE_H_
#define _AXE_H_

#include "../Bullet.h"



class Axe : public Bullet
{
private:
	double slowDownPercentage = 0.3;      //��������Ķ�������
	double slowDownDuration = 1;          //���ټ����ĳ���ʱ��

public:
	Axe();
	~Axe() = default;

	void OnCollide(Enemy*) override;
};

#endif
