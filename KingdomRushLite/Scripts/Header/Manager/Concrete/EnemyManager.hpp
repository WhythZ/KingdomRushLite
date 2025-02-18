#ifndef _ENEMY_MANAGER_HPP_
#define _ENEMY_MANAGER_HPP_

#include <vector>
#include <SDL.h>

#include "../Manager.hpp"
#include "../../Enemy/Enemy.hpp"

class EnemyManager :public Manager<EnemyManager>
{
	friend class Manager<EnemyManager>;

public:
	//记录所有应当在游戏场景中的怪物指针（指向的都是子类）
	typedef std::vector<Enemy*> EnemyList;

private:
	EnemyList enemyList;                   //当前在场上的怪物实例列表

public:
	void OnUpdate(double);                 //帧更新
	void OnRender(SDL_Renderer*);          //图像渲染

	void SpawnEnemy(EnemyType, int);       //在某个出生点处生成敌人

	bool IsEnemyCleaned() const;           //判断场上敌人是否清空
	const EnemyList& GetEnemyList() const; //获取场上敌人列表

private:
	EnemyManager() = default;
	~EnemyManager();

	void ProcessCollisionBullet();         //进行与投射物的碰撞的判定
	void ProcessCollisionHome();           //进行与家的碰撞的判定
	
	void RemoveDeadEnemies();              //移除掉死亡敌人
};

#endif
