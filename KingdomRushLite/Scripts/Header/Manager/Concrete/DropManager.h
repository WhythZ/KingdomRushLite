#ifndef _DROP_MANAGER_H_
#define _DROP_MANAGER_H_

#include "../Manager.hpp"
#include "../../Drop/Drop.h"

class DropManager :public Manager<DropManager>
{
	friend class Manager<DropManager>;

public:
	typedef std::vector<Drop*> DropList;

private:
	DropList dropList;                                    //存放当前场景中的掉落物实例指针

public:
	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	void SpawnDrop(DropType, const Vector2&);             //在某位置实例化一个掉落物
	void TrySpawnDrop(DropType, const Vector2&, double);  //以一定概率实例化一个掉落物

	const DropList& GetDropList() const;                  //获取场上掉落物列表

private:
	DropManager() = default;
	~DropManager();

	void RemoveInvalidDrops();                            //移除所有无效掉落物
};

#endif
