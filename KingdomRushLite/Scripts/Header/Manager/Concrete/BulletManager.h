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
	BulletList bulletList;                    //��ŵ�ǰ�����е��ӵ�ʵ��ָ��

public:
	void OnUpdate(double);                    //���³����е������ӵ�����
	void OnRender(SDL_Renderer*);             //��Ⱦ�����е������ӵ�����

	//�����ӵ��ķ������ֱ����ӵ����͡���ʼ����λ�á������ٶȡ��ӵ��˺����ٶ����˺���������ȼ���̬��أ�
	void FireBullet(BulletType, const Vector2&, const Vector2&, double);

	const BulletList& GetBulletList() const;  //�����ṩ��ȡ�����ӵ��Ľӿ�

private:
	BulletManager() = default;
	~BulletManager();

	void RemoveInvalidBullets();              //�Ƴ�������Ч�ӵ�
};

#endif
