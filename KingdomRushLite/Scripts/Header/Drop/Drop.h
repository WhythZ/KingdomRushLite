#ifndef _DROP_H_
#define _DROP_H_

#include "../Math/Vector2.h"
#include "../Math/Timer.h"
#include "DropType.h"

class Drop
{
protected:
	DropType type = DropType::None;

	#pragma region Transform
	Vector2 size;               //������ĳߴ�
	Vector2 position;           //��������ֵ�λ��
	#pragma endregion

	#pragma region Properties
	Vector2 velocity;           //�������Ǵӳ�ʼλ�����ϵ������ģ�����������ٶ�����
	double gravity = 10;        //�������������������ģ����ٶȸı��ǿ��
	#pragma endregion

private:
	bool isValid = true;        //�������Ƿ�Ӧ�����������ڳ�����
	bool isPoping = false;      //�������Ƿ��ڵ�����״̬����ʱ�ٶȲ�ӦΪ0��

	double popTime = 0.75;      //������������ʱ��
	double existTime = 10;      //��������ڵ�ʱ��
	Timer popTimer;             //���Ƶ�����ֹͣ������ģ�������ϵ�������׹������Ч��
	Timer disappearTimer;       //���Ƶ����ﾭ��һ��ʱ����Զ���ʧ

public:
	Drop();
	~Drop() = default;

	void OnUpdate(double);
	void OnRender(SDL_Renderer);
	void OnCollide();
};

#endif
