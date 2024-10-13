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
	//��¼����Ӧ������Ϸ�����еĹ���ָ�루ָ��Ķ������ࣩ
	typedef std::vector<Enemy*> EnemyList;

private:
	EnemyList enemyList;

public:
	void OnUpdate(double);          //֡����
	void OnRender(SDL_Renderer*);   //ͼ����Ⱦ

private:
	EnemyManager() = default;
	~EnemyManager();

	void ProcessCollisionBullet();  //������Ͷ�������ײ���ж�
	void ProcessCollisionHome();    //������ҵ���ײ���ж�
	void RemoveDeadEnemies();       //�Ƴ�����������
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
	//��ȡ�����ڵ���Ƭ�����
	static const SDL_Point& _homePt = ConfigManager::GetInstance()->map.GetHomePoint();
	//��ȡ��Ƭ��ͼ�������Ϸ���ڵ���ȾRect
	static const SDL_Rect& _mapRect = ConfigManager::GetInstance()->mapRect;
	//ͨ����������������������ҵ���ͼ�����ϽǶ����ʵ�����꣨Vector2��������ʵ������㣬��SDL_Point����ɢ�Ĵ���λ�ķ�ʵ������㣩
	static const Vector2 _homePos = {
		_mapRect.x + _homePt.x * TILE_SIZE,
		_mapRect.y + _homePt.y * TILE_SIZE,
	};
	#pragma endregion

	//���������ڳ��ĵ��ˣ�������Ƿ������ң������˾ʹ�������������յ����˺�
	for (Enemy* _enemy : enemyList)
	{
		//�Ѿ����˵Ĳ���
		if (!_enemy->IsAlive()) continue;

		//��ȡ���˵�λ�ã���ͼ�����ĵ��λ�ã�
		const Vector2 _pos = _enemy->GetPosition();

		//������˵����ĵ㴦�ڼҵ���ͼ���˴�����ͼ������ײ�䣩
		if (_pos.x >= _homePos.x
			&& _pos.y >= _homePos.y
			&& _pos.x <= _homePos.x + TILE_SIZE
			&& _pos.y <= _homePos.y + TILE_SIZE)
		{
			//��������
			_enemy->Kill();
			//�����˺�
			HomeManager::GetInstance()->DecreaseHealthBy(_enemy->GetAttackDamage());
		}
	}
}

void EnemyManager::ProcessCollisionHome()
{

}

void EnemyManager::RemoveDeadEnemies()
{
	//����remove_if�����б�����Lambda�ķ��ص�bool����true��Ԫ��ͳͳ����enemyList�б�������ĩβ����������һ��ָ���һ��true��Ԫ�صĵ�����
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

	//ɾ�����������ĵ��ˣ���ʱ��enemyList��remove_if�������£����������ĵ���ָ������б�ĩβ
	enemyList.erase(_begin, enemyList.end());
}

#endif
