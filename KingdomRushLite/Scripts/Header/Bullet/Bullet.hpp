#ifndef _BULLET_HPP_
#define _BULLET_HPP_

#include "../Math/Vector2.hpp"
#include "../Animation/Animation.hpp"
#include "../Manager/Concrete/ConfigManager.hpp"
#include "../Enemy/Enemy.hpp"

class Bullet
{
protected:
	#pragma region Transform
	Vector2 size;                           //�ӵ��ĳߴ�
	Vector2 position;                       //�ӵ���λ��
	#pragma endregion

	#pragma region Properties
	Vector2 velocity;                       //�ӵ��˶����ٶ�
	float damage = 0;                       //�ӵ��ܶԵ�����ɵ��˺�
	float damageRange = -1;                 //�ӵ�����˺���AOE��Χ��
	#pragma endregion

	#pragma region Animation
	Animation animFlying;                   //�ӵ�����ʱ�Ķ���
	bool canRotate;                         //�ӵ��Ƿ����ת�Զ�����Ӱ��
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
	double GetDamageRange() const;          //��ȡ�ӵ��˺���Χ�������˺���ײ���
	bool CanCollide() const;                //��ѯ�ӵ��Ŀ���ײ���
	bool IsValid() const;                   //��ѯ�ӵ���ʧЧ���
};

void Bullet::SetPosition(const Vector2& _position)
{
	position = _position;
}

void Bullet::SetVelocity(const Vector2& _velocity)
{
	velocity = _velocity;

	//����ӵ�������ת�����迼���ٶ������ķ��򣬷���ֻ�������С����
	if (canRotate)
	{
		//�����Ǻ�����ȡ����
		double _radian = std::atan2(velocity.y, velocity.x);
		//ת��Ϊŷ���Ƕȣ��˴���ʱʹ��Ӳ�����ʾ\pi
		rotationAngle = _radian * 180 / 3.1415926;
	}
}

void Bullet::SetDamage(double _damage)
{
	damage = _damage;
}

void Bullet::OnUpdate(double _delta)
{
	//�����ӵ�����ʱ�Ķ���
	animFlying.OnUpdate(_delta);

	//�����ٶ����������ӵ�λ�ã������ϵ�λʱ��ʹ��ʵ���ƶ�������֡���޹�
	position += velocity * _delta;

	//��Ե��⣬���ӵ��ɳ���ͼ�����õ�ͼRect���м춨����ʱ���
	static const SDL_Rect _mapRect = ConfigManager::Instance()->mapRect;
	if (position.x <= _mapRect.x + size.x / 2
		|| position.x >= _mapRect.x + _mapRect.w - size.x / 2
		|| position.y >= _mapRect.y + size.y / 2
		|| position.y <= _mapRect.y + _mapRect.h - size.y / 2)
	{
		Invalidate();
	}
}

void Bullet::OnRender(SDL_Renderer* _renderer)
{
	//�ӵ���ͼ�����ϽǶ������꣨����Bullet������øú���ʱ���Ḵ�øþ�̬�����������ظ����������ٵĿ�����
	static SDL_Point _point;
	_point.x = (int)(position.x - size.x / 2);
	_point.y = (int)(position.y - size.y / 2);

	//���õ�ǰ��������Ⱦ��ע�⴫������ת�Ƕ�
	animFlying.OnRender(_renderer, _point, rotationAngle);
}

void Bullet::OnCollide(Enemy* _enemy)
{
	//��ĳ��������ײ���ӵ���ʧ�Ҳ����ظ�������ײ
	Invalidate();
	DisableCollide();

	//�Ա���ײ���ĵ��˽����˺��춨
}

void Bullet::DisableCollide()
{
	canCollide = false;
}

void Bullet::Invalidate()
{
	isValid = false;
	//ʧЧ��ͬʱҲҪȡ����ײ����Ϊ�ӵ�ʧЧʱ��ײ����δʧЧ��һ��ʱ����δ���е��ˣ�
	DisableCollide();
}

const Vector2& Bullet::GetSize() const
{
	return size;
}

const Vector2& Bullet::GetPosition() const
{
	return position;
}

double Bullet::GetDamage() const
{
	return damage;
}

double Bullet::GetDamageRange() const
{
	return damageRange;
}

bool Bullet::CanCollide() const
{
	return canCollide;
}

bool Bullet::IsValid() const
{
	return isValid;
}

#endif
