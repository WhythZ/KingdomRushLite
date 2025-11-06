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
	EnemyPool enemyPool;                   //当前在场上的怪物实例列表

public:
	void OnUpdate(double);                 //帧更新
	void OnRender(SDL_Renderer*);          //图像渲染

	void SpawnEnemy(EnemyType, int);       //在某个出生点处生成敌人
	EnemyPool& GetEnemyPool();             //获取场上敌人列表

	bool IsEnemyCleaned() const;           //判断场上敌人是否清空

private:
	EnemyManager() = default;
	~EnemyManager() = default;

	void UpdateCollisionBullet();          //进行与投射物的碰撞的判定
	void UpdateCollisionHome();            //进行与家的碰撞的判定
};

#endif
