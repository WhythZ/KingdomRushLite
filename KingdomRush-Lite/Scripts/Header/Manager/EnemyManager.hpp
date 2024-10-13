#ifndef _ENEMY_MANAGER_HPP_
#define _ENEMY_MANAGER_HPP_

#include <vector>
#include <SDL.h>
#include "Manager.hpp"
#include "../Enemy/Enemy.hpp";
#include "ConfigManager.hpp"
#include "HomeManager.hpp"
#include "AudioManager.hpp"

class EnemyManager :public Manager<EnemyManager>
{
	friend class Manager<EnemyManager>;

public:
	//记录所有应当在游戏场景中的怪物指针（指向的都是子类）
	typedef std::vector<Enemy*> EnemyList;

private:
	EnemyList enemyList;

public:
	void OnUpdate(double);          //帧更新
	void OnRender(SDL_Renderer*);   //图像渲染

private:
	EnemyManager() = default;
	~EnemyManager();

	void ProcessCollisionBullet();  //进行与投射物的碰撞的判定
	void ProcessCollisionHome();    //进行与家的碰撞的判定
	void RemoveDeadEnemies();       //移除掉死亡敌人
};

void EnemyManager::OnUpdate(double _delta)
{
	for (Enemy* _enemy : enemyList)
		_enemy->OnUpdate(_delta);
}

void EnemyManager::OnRender(SDL_Renderer* _renderer)
{
	for (Enemy* _enemy : enemyList)
		_enemy->OnRender(_renderer);
}

EnemyManager::~EnemyManager()
{
	for (Enemy* _enemy : enemyList)
		delete _enemy;
}

void EnemyManager::ProcessCollisionBullet()
{
	#pragma region GetHomePosition
	//获取家所在的瓦片坐标点
	static const SDL_Point& _homePt = ConfigManager::GetInstance()->map.GetHomePoint();
	//获取瓦片地图相对于游戏窗口的渲染Rect
	static const SDL_Rect& _mapRect = ConfigManager::GetInstance()->mapRect;
	//通过上述两个东西，计算出家的贴图的左上角顶点的实际坐标（Vector2是连续的实际坐标点，而SDL_Point是离散的带单位的非实际坐标点）
	static const Vector2 _homePos = {
		_mapRect.x + _homePt.x * TILE_SIZE,
		_mapRect.y + _homePt.y * TILE_SIZE,
	};
	#pragma endregion

	//遍历所有在场的敌人，检测其是否碰到家，碰到了就处死，并计算家收到的伤害
	for (Enemy* _enemy : enemyList)
	{
		//已经死了的不算
		if (!_enemy->IsAlive()) continue;

		//获取敌人的位置（贴图的中心点的位置）
		const Vector2 _pos = _enemy->GetPosition();

		//如果敌人的中心点处于家的贴图（此处把贴图当作碰撞箱）
		if (_pos.x >= _homePos.x
			&& _pos.y >= _homePos.y
			&& _pos.x <= _homePos.x + TILE_SIZE
			&& _pos.y <= _homePos.y + TILE_SIZE)
		{
			//处死敌人
			_enemy->Kill();
			//计算伤害
			HomeManager::GetInstance()->DecreaseHealthBy(_enemy->GetAttackDamage());
		}
	}
}

void EnemyManager::ProcessCollisionHome()
{

}

void EnemyManager::RemoveDeadEnemies()
{
	//函数remove_if遍历列表，按照Lambda的返回的bool，将true的元素统统放入enemyList列表容器的末尾，并将返回一个指向第一个true的元素的迭代器
	auto _begin = std::remove_if(enemyList.begin(), enemyList.end(),
		[](const Enemy* _enemy)
		{
			if (!_enemy->IsAlive())
			{
				delete _enemy;
				return true;
			}
			return false;
		});

	//删除所有死亡的敌人，此时的enemyList在remove_if的排列下，所有死亡的敌人指针均在列表末尾
	enemyList.erase(_begin, enemyList.end());
}

#endif
