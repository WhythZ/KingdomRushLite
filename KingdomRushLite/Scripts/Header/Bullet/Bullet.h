#ifndef _BULLET_H_
#define _BULLET_H_

#include "BulletType.h"
#include "../Math/Vector2.h"
#include "../Animation/Animation.h"
#include "../Enemy/Enemy.h"

class Bullet
{
protected:
	BulletType type = BulletType::None;     //�ӵ�������

	#pragma region Transform
	Vector2 size;                           //�ӵ��ĳߴ�
	Vector2 position;                       //�ӵ���λ��
	#pragma endregion

	#pragma region Properties
	Vector2 velocity;                       //�ӵ��˶����ٶ�
	double damage = 0;                      //�ӵ��ܶԵ�����ɵ��˺�
	double damageRadius = -1;               //�ӵ�����˺���AOE��Χ�뾶
	#pragma endregion

	#pragma region Animation
	Animation animFlying;                   //�ӵ�����ʱ�Ķ���
	bool canRotate = false;                 //�ӵ��Ƿ����ת�Զ�����Ӱ��
	#pragma endregion

private:
	bool isValid = true;                    //�����ӵ��Ƿ�Ӧ����������
	bool canCollide = true;                 //�ӵ��Ƿ���Ա���ײ���˺��춨����������ײ��һ֡������������ײ�󶯻�ʱ��֡��Ӧ�����춨��
	double rotationAngle = 0;               //�����ӵ���׼�����ŷ���Ƕȣ����ڶ����Ĵ���

public:
	Bullet() = default;
	~Bullet() = default;
	void SetPosition(const Vector2&);       //�趨�ӵ��ķ���λ��
	void SetVelocity(const Vector2&);       //�趨�ӵ�������ٶ�����
	void SetDamage(double);                 //�趨�ӵ����е��˵��˺�

	virtual void OnUpdate(double);
	virtual void OnRender(SDL_Renderer*);
	virtual void OnCollide(Enemy*);         //�ӵ���ײ���ĺ���

	void DisableCollide();                  //ȡ���ӵ��Ŀ���ײ�������˲ʱ�˺����ǳ����˺�
	void Invalidate();                      //ʹ���ӵ���ʧ

	const Vector2& GetSize() const;         //��ȡ�ӵ��ߴ磬����������ײ���
	const Vector2& GetPosition() const;     //��ȡ�ӵ�λ��
	double GetDamage() const;               //��ȡ�ӵ��˺�
	double GetDamageRadius() const;         //��ȡ�ӵ��˺���Χ�������˺���ײ���
	bool CanCollide() const;                //��ѯ�ӵ��Ŀ���ײ���
	bool IsValid() const;                   //��ѯ�ӵ���ʧЧ���
};

#endif
