#ifndef _BULLET_MANAGER_HPP_
#define _BULLET_MANAGER_HPP_

#include <vector>
#include <SDL.h>

#include "../Manager.hpp"
#include "../../Bullet/Bullet.hpp"
#include "../../Bullet/BulletType.hpp"
#include "../../Bullet/Concrete/Arrow.hpp"
#include "../../Bullet/Concrete/Axe.hpp"
#include "../../Bullet/Concrete/Shell.hpp"

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
	void FireBullet(BulletType, const Vector2&, const Vector2&, float);

	const BulletList& GetBulletList() const;  //对外提供获取场上子弹的接口

private:
	BulletManager() = default;
	~BulletManager();

	void RemoveInvalidBullets();              //移除无效子弹
};

BulletManager::~BulletManager()
{
	for (Bullet* _bullet : bulletList)
		delete _bullet;
}

void BulletManager::OnUpdate(double _delta)
{
	for (Bullet* _bullet : bulletList)
		_bullet->OnUpdate(_delta);

	//检查无效的子弹并从场景中移除
	RemoveInvalidBullets();
}

void BulletManager::OnRender(SDL_Renderer* _renderer)
{
	for (Bullet* _bullet : bulletList)
		_bullet->OnRender(_renderer);
}

void BulletManager::FireBullet(BulletType _type, const Vector2& _position, const Vector2& _velocity, float _damager)
{
	Bullet* _bullet = nullptr;
	switch (_type)
	{
	case BulletType::Arrow:
		_bullet = new Arrow();
		break;
	case BulletType::Axe:
		_bullet = new Axe();
		break;
	case BulletType::Shell:
		_bullet = new Shell();
		break;
	default:
		break;
	}
	if (_bullet == nullptr)
		return;

	//设置子弹初始发射位置、速度、伤害
	_bullet->SetPosition(_position);
	_bullet->SetVelocity(_velocity);
	_bullet->SetDamage(_damager);

	//将新生成的子弹存入列表
	bulletList.push_back(_bullet);
}

const BulletManager::BulletList& BulletManager::GetBulletList() const
{
	return bulletList;
}

void BulletManager::RemoveInvalidBullets()
{
	//函数remove_if遍历列表，按照Lambda的返回的bool，将true的元素统统放入列表容器的末尾，并将返回一个指向第一个true的元素的迭代器
	auto _begin = std::remove_if(bulletList.begin(), bulletList.end(),
		[](const Bullet* _bullet)
		{
			if (!_bullet->IsValid())
			{
				delete _bullet;
				return true;
			}
			return false;
		});

	//删除所有无效的子弹，此时的列表在remove_if的排列下，所有无效的子弹指针均在列表末尾
	bulletList.erase(_begin, bulletList.end());
}

#endif
