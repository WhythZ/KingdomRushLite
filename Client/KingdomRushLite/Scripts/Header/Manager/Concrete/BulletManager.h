#ifndef _BULLET_MANAGER_H_
#define _BULLET_MANAGER_H_

#include <vector>
#include <SDL.h>

#include "../Manager.hpp"
#include "../../Bullet/Bullet.h"
#include "../../Bullet/BulletType.h"

class BulletManager : public Manager<BulletManager>
{
	friend class Manager<BulletManager>;

public:
	typedef std::vector<Bullet*> BulletList;

private:
	BulletList bulletList;                    //存放当前场景中的子弹实例指针

public:
	void OnUpdate(double);                    //更新场景中的所有子弹对象
	void OnRender(SDL_Renderer*);             //渲染场景中的所有子弹对象

	//发射子弹的方法，分别传入子弹类型、初始发射位置、发射速度、子弹伤害（速度与伤害与防御塔等级动态相关）
	void FireBullet(BulletType, const Vector2&, const Vector2&, double);

	const BulletList& GetBulletList() const;  //对外提供获取场上子弹的接口

private:
	BulletManager() = default;
	~BulletManager();

	void RemoveInvalidBullets();              //移除所有无效子弹
};

#endif
