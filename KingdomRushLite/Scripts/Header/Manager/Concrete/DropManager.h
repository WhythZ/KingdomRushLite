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
	DropList dropList;                                    //��ŵ�ǰ�����еĵ�����ʵ��ָ��

public:
	void OnUpdate(double);
	void OnRender(SDL_Renderer*);

	void SpawnDrop(DropType, const Vector2&);             //��ĳλ��ʵ����һ��������
	void TrySpawnDrop(DropType, const Vector2&, double);  //��һ������ʵ����һ��������

	const DropList& GetDropList() const;                  //��ȡ���ϵ������б�

private:
	DropManager() = default;
	~DropManager();

	void RemoveInvalidDrops();                            //�Ƴ�������Ч������
};

#endif
