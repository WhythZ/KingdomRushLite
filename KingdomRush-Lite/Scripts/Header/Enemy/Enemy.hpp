#ifndef _ENEMY_HPP_
#define _ENEMY_HPP_

#include <functional>
#include "../Animation/Animation.hpp"
#include "../Vector/Vector2.hpp"
#include "../Timer/Timer.hpp"
#include "../Map/Route.hpp"
#include "EnemyType.hpp"
#include "../Manager/ConfigManager.hpp"
#include "../Manager/ResourceManager.hpp"

class Enemy
{
public:
	//�������ʹ�ü��ܵĻص�����������һ�������ͷ��ߵ�ָ�����
	typedef std::function<void(Enemy*)> SkillCallBack;

private:
	Vector2 position;                        //����λ��
	Vector2 velocity;                        //�ٶ�ʸ��
	Vector2 direction;                       //�н�����

	bool isAlive = true;                     //��¼�����Ƿ���

	#pragma region Animation
	Animation* animCurrent;                  //ָ��ǰ���õĶ���
	bool isShowSketch;                       //�Ƿ����ܻ���ף������Ƽ�Ӱ����״̬
	Timer sketchTimer;                       //�����ܻ������Ч����ʱ��
	#pragma endregion

	#pragma region Route
	const Route* route = nullptr;            //�ù����н�����Ƭ·��
	int currentTileIdx = 0;                  //��ǰ���ﴦ��·���ϵ���Ƭ������
	Vector2 targetTilePosition;              //��Ҫʻ�����һ����Ƭ������λ��
	#pragma endregion

	#pragma region Skill
	SkillCallBack skillRecoverTrigger;       //�ͷŻָ����ܵĻص�����
	Timer skillRecoverCooldowndTimer;        //�ָ������ͷ���ȴ�ļ�ʱ��
	Timer skillRecoverFinishedTimer;         //�����ͷŻָ�������Ҫ�ķѶ೤ʱ��
	#pragma endregion

protected:
	EnemyType type;                          //���˵�����

	#pragma region Animation
	Vector2 size;                            //����ͼƬ�Ķ�ά�ߴ��С
	Animation animUp;                        //���Ϸ���Ķ���
	Animation animDown;                      //���·���Ķ���
	Animation animLeft;                      //������Ķ���
	Animation animRight;                     //���ҷ���Ķ���
	Animation animUpSketch;                  //���Ϸ�����ܻ���Ӱ����
	Animation animDownSketch;                //���·�����ܻ���Ӱ����
	Animation animLeftSketch;                //��������ܻ���Ӱ����
	Animation animRightSketch;               //���ҷ�����ܻ���Ӱ����
	#pragma endregion
	
	#pragma region BasicStats
	double healthMaximum = 0;                //�������ֵ����ȡ�����ļ���
	double healthCurrent = 0;                //��ǰ����ֵ����̬���м�¼��

	double speedMaximum = 0;                 //����ƶ����ʱ�������ȡ�����ļ���
	double speedCurrent = 0;                 //��ǰ�ƶ����ʱ�������̬���м�¼��

	double damage = 0;                       //���˿��ԶԷ��ص�λ��ɵ��˺�

	double rewardRatio = 0;                  //�����������
	#pragma endregion

	#pragma region SkillStats
	double skillRecoverCooldown = 0;         //�ָ����ܵ���ȴ
	double skillRecoverRange = 0;            //�ָ����ܵķ�Χ
	double skillRecoverIntensity = 0;        //�ָ����ܵ�ǿ��
	#pragma endregion

public:
	Enemy();
	~Enemy() = default;

	void OnUpdate(double);                   //֡���º���

private:
	void UpdateTargetTilePosition();         //����Ŀ����Ƭ��λ��
};

Enemy::Enemy()
{
	#pragma region Animation
	//�ܻ���������һ�μ���
	sketchTimer.SetOneShot(true);
	//�����ܻ����׵ĳ���ʱ�䣬�̶̵ľͺ�
	sketchTimer.SetWaitTime(0.1);
	//���ûص�����
	sketchTimer.SetTimeOutTrigger(
		[&]()
		{
			isShowSketch = false;
		}
	);
	#pragma endregion
	
	#pragma region Skill
	//�ָ�������ȴһ�����ͻ��ٴ��ͷţ����Բ��ǵ��δ���
	skillRecoverCooldowndTimer.SetOneShot(false);
	//��ȴ�������ͷŻָ����ܣ��������˰�װ���Ӧ�ص���������������
	skillRecoverCooldowndTimer.SetTimeOutTrigger(
		[&]()
		{
			//���ܵ��ͷ���Ϊ�Լ�
			skillRecoverTrigger(this);
		}
	);

	//ÿ���������ܱ��������ͷŸü��ܵĹ������ٻ�ֹͣ��ͨ���ü�ʱ���������ʱ�ָ���ʼ�ٶȣ������ǵ��δ���
	skillRecoverFinishedTimer.SetOneShot(true);
	skillRecoverFinishedTimer.SetTimeOutTrigger(
		[&]()
		{
			speedCurrent = speedMaximum;
		}
	);
	#pragma endregion
}

void Enemy::OnUpdate(double _delta)
{
	//���µ�ǰ����
	animCurrent->OnUpdate(_delta);

	//���¸���ʱ��
	sketchTimer.OnUpdate(_delta);
	skillRecoverCooldowndTimer.OnUpdate(_delta);
	skillRecoverFinishedTimer.OnUpdate(_delta);

	#pragma region MoveOnDistance
	//�����֡�������µ�_deltaʱ���ڣ������й��������ƶ��ľ���
	Vector2 _moveDistance = velocity * _delta;
	//���������һ��Ŀ��ש��ľ��룬��Ŀ��λ���뵱ǰλ�õĲ�
	Vector2 _targetDistance = targetTilePosition - position;
	//��һ֡ʵ���ƶ��ľ��벻Ӧ�����ڵ���ǰĿ���ľ��루���ٶȽ�С����ܳ���С�ڵ���������޷����������֡���º����ܵ���Ŀ�괦�ģ�
	position += (_moveDistance < _targetDistance) ? _moveDistance : _targetDistance;
	//�����ǰλ�þ���Ŀ��ص�ľ��볤�ȣ��ڱ궨�߶��£�������0����˵�������˸�Ŀ��ص㣬��ʱҪ����Ŀ��λ��
	if (_targetDistance.ApproxZero())
		UpdateTargetTilePosition();
	#pragma endregion
}

void Enemy::UpdateTargetTilePosition()
{
	//��ȡ·����ά��Ƭ�����б�
	const Route::TilePointList& _tilePointList = route->GetTilePointList();
	
	//Ŀ����Ƭ���������ǵ�ǰ������Ƭ��������һ��
	int _targetTileIdx = currentTileIdx + 1;

	//�����ǰû�г���·������Ƭ�������������Ѱ����һ��Ŀ��������Ļ�ϵ�λ��
	if (_targetTileIdx < _tilePointList.size())
	{
		//��ȡĿ����Ƭ�Ķ�άSDL_Point���꣬����Vector2��ͬ��ǰ������ɢ�ģ�����TILE_SIZE���ܱ�ʾʵ�ʾ��룩�����꣬�����������ľ����������
		const SDL_Point& _targetTilePoint = _tilePointList[_targetTileIdx];

		//��ȡ��̬��������Ƭ��ͼ��Ⱦ����Ϸ�����е�λ��Rect�����ڶ�λ·���ϵ�Ŀ��������Ϸ���ڵ�λ��
		static const SDL_Rect& _mapRect = ConfigManager::GetInstance()->mapRect;

		//�ӵ�ͼ���Ͻǿ�ʼ��������ά��Ƭ�����ɢ����Ѱ�ҵ���·���ϵģ�Ŀ����Ƭ���϶���ľ������꣬�ټ��ϰ��TILE_SIZE�õ�Ŀ����Ƭ��������Ϸ�����ϵ�����
		position.x = _mapRect.x + (_targetTilePoint.x * TILE_SIZE) + TILE_SIZE / 2;
		position.y = _mapRect.y + (_targetTilePoint.y * TILE_SIZE) + TILE_SIZE / 2;


	}
}

#endif
