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
	BulletList bulletList;                    //��ŵ�ǰ�����е��ӵ�ʵ��ָ��

public:
	void OnUpdate(double);                    //���³����е������ӵ�����
	void OnRender(SDL_Renderer*);             //��Ⱦ�����е������ӵ�����

	//�����ӵ��ķ������ֱ����ӵ����͡���ʼ����λ�á������ٶȡ��ӵ��˺����ٶ����˺���������ȼ���̬��أ�
	void FireBullet(BulletType, const Vector2&, const Vector2&, float);

	const BulletList& GetBulletList() const;  //�����ṩ��ȡ�����ӵ��Ľӿ�

private:
	BulletManager() = default;
	~BulletManager();

	void RemoveInvalidBullets();              //�Ƴ���Ч�ӵ�
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

	//�����Ч���ӵ����ӳ������Ƴ�
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

	//�����ӵ���ʼ����λ�á��ٶȡ��˺�
	_bullet->SetPosition(_position);
	_bullet->SetVelocity(_velocity);
	_bullet->SetDamage(_damager);

	//�������ɵ��ӵ������б�
	bulletList.push_back(_bullet);
}

const BulletManager::BulletList& BulletManager::GetBulletList() const
{
	return bulletList;
}

void BulletManager::RemoveInvalidBullets()
{
	//����remove_if�����б�����Lambda�ķ��ص�bool����true��Ԫ��ͳͳ�����б�������ĩβ����������һ��ָ���һ��true��Ԫ�صĵ�����
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

	//ɾ��������Ч���ӵ�����ʱ���б���remove_if�������£�������Ч���ӵ�ָ������б�ĩβ
	bulletList.erase(_begin, bulletList.end());
}

#endif
