#ifndef _DROP_H_
#define _DROP_H_

#include "DropType.h"
#include "../Infra/Vector2.h"
#include "../Infra/Timer.h"
#include "../Infra/Animation.h"

class Drop
{
protected:
	DropType type = DropType::None;
	SDL_Point size;                    //������ĳߴ�
	Vector2 position;                  //��������ֵ�λ��
	Vector2 velocity;                  //�������Ǵӳ�ʼλ�����ϵ������ģ�����������ٶ�����

	#pragma region Properties
	double gravity = 10;               //��������������ٶ�
	double popDuration = 1;            //������������ʱ��
	double existDuration = 10;         //��������ڵ�ʱ��
	double initSpeedX = 2;             //����������ʱ�ĳ�ʼˮƽ����
	double initSpeedY = 4;             //����������ʱ�ĳ�ʼ��������
	double floatingFrequency = 4;      //�����Ǻ����Ա�����ˣ����Ƶ������ڵ����ϵĲ���Ƶ��
	double floatingAmplitude = 16;     //�����Ǻ���ֵ��ˣ����Ƶ������ڵ����ϵĲ������
	#pragma endregion

	#pragma region Animation
	Animation animIdle;                //����������״̬����
	#pragma endregion

private:
	bool isValid = true;               //�������Ƿ�Ӧ�����������ڳ�����
	bool isPoping = false;             //�������Ƿ��ڵ�����״̬����ʱ�ٶȲ�ӦΪ0��

	Timer popTimer;                    //���Ƶ�����ֹͣ������ģ�������ϵ�������׹������Ч��
	Timer disappearTimer;              //���Ƶ����ﾭ��һ��ʱ����Զ���ʧ

public:
	Drop();
	~Drop() = default;
	void SetPosition(const Vector2&);

	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	virtual void OnCollide();

	void Invalidate();

	bool IsValid() const;
	const Vector2& GetPosition() const;
	const SDL_Point& GetSize() const;
};

#endif
