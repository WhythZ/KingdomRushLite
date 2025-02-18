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
	//��¼����Ӧ������Ϸ�����еĹ���ָ�루ָ��Ķ������ࣩ
	typedef std::vector<Enemy*> EnemyList;

private:
	EnemyList enemyList;                   //��ǰ�ڳ��ϵĹ���ʵ���б�

public:
	void OnUpdate(double);                 //֡����
	void OnRender(SDL_Renderer*);          //ͼ����Ⱦ

	void SpawnEnemy(EnemyType, int);       //��ĳ�������㴦���ɵ���

	bool IsEnemyCleaned() const;           //�жϳ��ϵ����Ƿ����
	const EnemyList& GetEnemyList() const; //��ȡ���ϵ����б�

private:
	EnemyManager() = default;
	~EnemyManager();

	void ProcessCollisionBullet();         //������Ͷ�������ײ���ж�
	void ProcessCollisionHome();           //������ҵ���ײ���ж�
	
	void RemoveDeadEnemies();              //�Ƴ�����������
};

#endif
