#ifndef _ENEMY_MANAGER_H_
#define _ENEMY_MANAGER_H_

#include <vector>
#include <SDL.h>

#include "../Manager.hpp"
#include "../../Enemy/Enemy.h"
#include "../../Infra/ObjectPool/EnemyPool.h"

class EnemyManager :public Manager<EnemyManager>
{
	friend class Manager<EnemyManager>;

private:
	EnemyPool enemyPool;                   //��ǰ�ڳ��ϵĹ���ʵ���б�

public:
	void OnUpdate(double);                 //֡����
	void OnRender(SDL_Renderer*);          //ͼ����Ⱦ

	void SpawnEnemy(EnemyType, int);       //��ĳ�������㴦���ɵ���
	EnemyPool& GetEnemyPool();             //��ȡ���ϵ����б�

	bool IsEnemyCleaned() const;           //�жϳ��ϵ����Ƿ����

private:
	EnemyManager() = default;
	~EnemyManager() = default;

	void UpdateCollisionBullet();          //������Ͷ�������ײ���ж�
	void UpdateCollisionHome();            //������ҵ���ײ���ж�
};

#endif
