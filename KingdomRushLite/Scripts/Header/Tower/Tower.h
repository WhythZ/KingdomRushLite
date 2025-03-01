#ifndef _TOWER_H_
#define _TOWER_H_

#include "TowerType.h"
#include "../Infra/Vector2.h"
#include "../Infra/Timer.h"
#include "../Infra/DirectionType.h"
#include "../Infra/Animation.h"
#include "../Bullet/BulletType.h"
#include "../Enemy/Enemy.h"

class Tower
{
protected:
	TowerType type = TowerType::None;          //������������
	SDL_Point size = { 0 };                    //������Ⱦ����ײ�ĳߴ�
	Vector2 position;                          //����������λ��

	#pragma region Animation
	Animation animIdleUp;              //�������ϵĶ���
	Animation animIdleDown;            //�������µĶ���
	Animation animIdleLeft;            //��������Ķ���
	Animation animIdleRight;           //�������ҵĶ���

	Animation animFireUp;              //�������ϵĶ���
	Animation animFireDown;            //�������µĶ���
	Animation animFireLeft;            //��������Ķ���
	Animation animFireRight;           //�������ҵĶ���
	#pragma endregion

	#pragma region Properties
	double fireCooldown = 0;                   //ÿ�η���ļ��
	double fireRadius = 0;                     //������Χ�뾶
	BulletType bulletType = BulletType::None;  //�ӵ�������
	double bulletDamage = 0;                   //�ӵ����˺�
	double bulletSpeed = 0;                    //�ӵ����ٶ�
	#pragma endregion

private:
	Animation* animCurrent = &animIdleDown;    //������Ĭ�϶���
	FacingDir facingDir = FacingDir::Down;     //������Ĭ�϶����ĳ���

	bool canFire = true;                       //�����Ƿ���Է����ӵ�
	Timer fireTimer;                           //�����ӵ��������Ķ�ʱ��

public:
	Tower();
	~Tower() = default;
	void SetPosition(const Vector2&);          //���÷�������λ��

	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	const SDL_Point& GetSize() const;          //��ȡ�������ĳߴ�
	const Vector2& GetPosition() const;        //��ȡ��������λ��
		
protected:
	void virtual OnFireBullet(Enemy*);         //�ӵ�����ʱ���߼�

private:
	void UpdateFacingDir(const Vector2&);      //���·���������
	void UpdateIdleAnim();                     //���ճ�����µ�ǰ����Ϊ��Ӧ���ö���
	void UpdateFireAnim();                     //���ճ�����µ�ǰ����Ϊ��Ӧ���𶯻�
	Enemy* FindTargetEnemy();                  //�ӵ����е��߼�
};

#endif
