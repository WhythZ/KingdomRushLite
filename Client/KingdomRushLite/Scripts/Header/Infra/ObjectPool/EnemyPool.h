#ifndef _ENEMY_POOL_H_
#define _ENEMY_POOL_H_

#include <SDL.h>

#include "ObjectPool.hpp"
#include "../../Enemy/Concrete/Slime.h"
#include "../../Enemy/Concrete/SlimeKing.h"
#include "../../Enemy/Concrete/Skeleton.h"
#include "../../Enemy/Concrete/Goblin.h"
#include "../../Enemy/Concrete/GoblinPriest.h"

//对象池的Enemy*迭代器，用于遍历敌人池中的所有敌人
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

//管理Enemy的派生类的大对象池
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

	//二者使得EnemyPool可被范围for循环遍历
	EnemyPoolIterator begin();                   //迭代器begin语义
	EnemyPoolIterator end();                     //迭代器end语义

	Enemy* Acquire(EnemyType);                   //获取一个特定类型的敌人对象

	bool NoBusyEnemy() const;                    //判断场上是否还有活跃的敌人

private:
	void RemoveDeadEnemies();                    //释放死亡敌人回到池中
};

#endif