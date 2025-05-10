#ifndef _ENEMY_POOL_H_
#define _ENEMY_POOL_H_

#include <SDL.h>

#include "ObjectPool.hpp"
#include "../../Enemy/Concrete/Slime.h"
#include "../../Enemy/Concrete/SlimeKing.h"
#include "../../Enemy/Concrete/Skeleton.h"
#include "../../Enemy/Concrete/Goblin.h"
#include "../../Enemy/Concrete/GoblinPriest.h"

//����ص�Enemy*�����������ڱ������˳��е����е���
class EnemyPoolIterator
{
private:
	std::vector<Enemy*> enemies;
	size_t index = 0;

public:
	EnemyPoolIterator(const std::vector<Enemy*>& _enemies, size_t _index = 0)
		: enemies(_enemies), index(_index) {}

	Enemy* operator*() const { return enemies[index]; }
	EnemyPoolIterator& operator++() { ++index; return *this; }
	bool operator!=(const EnemyPoolIterator& _other) const { return index != _other.index; }
};

//����Enemy��������Ĵ�����
class EnemyPool
{
private:

	ObjectPool<Slime> slimePool;
	ObjectPool<SlimeKing> slimeKingPool;
	ObjectPool<Skeleton> skeletonPool;
	ObjectPool<Goblin> goblinPool;
	ObjectPool<GoblinPriest> goblinPriestPool;

public:
	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	//����ʹ��EnemyPool�ɱ���Χforѭ������
	EnemyPoolIterator begin();                   //������begin����
	EnemyPoolIterator end();                     //������end����

	Enemy* Acquire(EnemyType);                   //��ȡһ���ض����͵ĵ��˶���

	bool NoBusyEnemy() const;                    //�жϳ����Ƿ��л�Ծ�ĵ���

private:
	void RemoveDeadEnemies();                    //�ͷ��������˻ص�����
};

#endif