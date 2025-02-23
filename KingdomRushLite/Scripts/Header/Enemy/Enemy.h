#ifndef _ENEMY_H_
#define _ENEMY_H_

#include <functional>
#include "../Infra/Animation.h"
#include "../Infra/Vector2.h"
#include "../Map/Route.h"
#include "EnemyType.h"

//������ٶ�ʧ���ٶȣ���λ��ÿ����Ƭ��
#define SLOW_DOWN_SPEED_LOST 0.3
//���ﱻ���ٵĳ���ʱ��
#define SLOW_DOWN_DURATION 1

class Enemy
{
public:
	//�������ʹ�ü��ܵĻص�����������һ�������ͷ��ߵ�ָ�����
	typedef std::function<void(Enemy*)> SkillCallBack;

protected:
	EnemyType type = EnemyType::None;           //���˵�����

	#pragma region Animation
	Vector2 spriteSize;                         //����ͼƬ�Ķ�ά�ߴ��С
	Animation animUp;                           //���Ϸ���Ķ���
	Animation animDown;                         //���·���Ķ���
	Animation animLeft;                         //������Ķ���
	Animation animRight;                        //���ҷ���Ķ���
	Animation animUpSketch;                     //���Ϸ�����ܻ���Ӱ����
	Animation animDownSketch;                   //���·�����ܻ���Ӱ����
	Animation animLeftSketch;                   //��������ܻ���Ӱ����
	Animation animRightSketch;                  //���ҷ�����ܻ���Ӱ����
	#pragma endregion

	#pragma region BasicStats
	double healthMaximum = 0;                   //�������ֵ����ȡ�����ļ���
	double healthCurrent = 0;                   //��ǰ����ֵ����̬���м�¼��

	double speedMaximum = 0;                    //����ƶ����ʱ�������ȡ�����ļ���
	double speedCurrent = 0;                    //��ǰ�ƶ����ʱ�������̬���м�¼��

	double attackDamage = 0;                    //���˿��ԶԷ��ص�λ���ң���ɵ��˺�

	double coinRatio = 0;                       //�����������
	#pragma endregion

	#pragma region Skill
	double skillRecoverCooldown = 0;            //�ָ����ܵ���ȴ
	double skillRecoverRadius = 0;              //�ָ����ܵķ�Χ�뾶
	double skillRecoverIntensity = 0;           //�ָ����ܵ�ǿ��

	Timer skillRecoverCooldowndTimer;           //�ָ������ͷ���ȴ�ļ�ʱ��
	#pragma endregion

private:
	Vector2 position;                           //����λ��
	Vector2 velocity;                           //�ٶ�ʸ��
	Vector2 direction;                          //�н�����

	bool isAlive = true;                        //��¼�����Ƿ���

	Timer speedRestoreTimer;                    //���ƹ���ָ���ʼ����

	#pragma region Animation
	Animation* animCurrent;                     //ָ��ǰ���õĶ���
	bool isShowSketch = false;                  //�Ƿ����ܻ���ף������Ƽ�Ӱ����״̬
	Timer sketchTimer;                          //�����ܻ������Ч����ʱ��
	#pragma endregion

	#pragma region Route
	const Route* route = nullptr;               //�ù����н�����Ƭ·��
	size_t targetTileIdx = 0;                   //�����Ŀ����Ƭ����
	Vector2 targetTilePosition;                 //��Ҫʻ�����һ����Ƭ������λ��
	#pragma endregion

	#pragma region Skill
	SkillCallBack skillRecoverTrigger;          //�ͷŻָ����ܵĻص�����
	#pragma endregion

public:
	Enemy();
	~Enemy() = default;
	void SetRecoverSkillTrigger(SkillCallBack); //���ûظ����ܵĻص�����
	void SetRoute(const Route*);                //�����н�·��
	void SetPosition(const Vector2&);           //���ó�ʼλ��

	void OnUpdate(double);                      //֡���º���
	void OnRender(SDL_Renderer*);               //��Ⱦ��ͼ����

	void IncreaseHealthBy(double);              //��������ֵ
	void DecreaseHealthBy(double);              //��������ֵ
	void SlowDown();                            //���ټ���
	void Kill();                                //��Ч���ù���

	bool IsAlive() const;                       //��ȡ������״̬
	double GetRouteProcess() const;             //��ȡ�����н�״��

	double GetHealth() const;                   //��ȡ��ǰѪ��
	const Vector2& GetSize() const;             //��ȡ��ͼ����ײ�䣩�ߴ�
	const Vector2& GetPosition() const;         //��ȡ����λ��
	const Vector2& GetTargetPosition() const;   //��ȡ�����Ŀ��λ��
	const Vector2& GetVelocity() const;         //��ȡ�����ٶ�ʸ��
	double GetAttackDamage() const;             //��ȡ�����ܶԼ���ɵ��˺�

	double GetSkillRecoverCooldown() const;     //��ȡ�ָ����ܵ���ȴ
	double GetSkillRecoverRadius() const;       //��ȡ�ָ����ܵķ�Χ�뾶
	double GetSkillRecoverIntensity() const;    //��ȡ�ָ����ܵ�ǿ��

private:
	void RefreshTargetTile();                   //����Ŀ��
};

#endif
