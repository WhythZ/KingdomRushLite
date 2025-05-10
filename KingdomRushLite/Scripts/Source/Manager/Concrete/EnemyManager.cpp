#include "../../../Header/Manager/Concrete/EnemyManager.h"
#include "../../../Header/Manager/Concrete/ProcessManager.h"
#include "../../../Header/Manager/Concrete/BulletManager.h"
#include "../../../Header/Enemy/Concrete/Slime.h"
#include "../../../Header/Enemy/Concrete/SlimeKing.h"
#include "../../../Header/Enemy/Concrete/Skeleton.h"
#include "../../../Header/Enemy/Concrete/Goblin.h"
#include "../../../Header/Enemy/Concrete/GoblinPriest.h"

void EnemyManager::OnUpdate(double _delta)
{
	//���¶�����ڵĻ�Ծ����
	enemyPool.OnUpdate(_delta);

	//��ײ���
	UpdateCollisionBullet();
	UpdateCollisionHome();
}

void EnemyManager::OnRender(SDL_Renderer* _renderer)
{
	enemyPool.OnRender(_renderer);
}

void EnemyManager::SpawnEnemy(EnemyType _type, int _spawnPointIdx)
{
	//�ӳ��л�ȡ�������
	Enemy* _enemy = enemyPool.Acquire(_type);
	//��ָ��������Чʹ��ָ����δָ��ʵ�����Ķ�����ֹͣ����
	if (_enemy == nullptr)
		return;

	#pragma region LocateSpawnPosition
	//��ʱ���ڴ洢λ����Ϣ
	static Vector2 _pos;
	//��ȡ��ͼRect���ڶ�λ
	static const SDL_Rect& _mapRect = ProcessManager::Instance()->mapRect;

	//��ȡ����·���أ�������������ĳ�����
	static const RoutePool& _spawnRoutePool = ProcessManager::Instance()->map.GetSpawnRoutePool();
	//��ȡ_spawnPointIdx��·�����ж�Ӧ�Ķ�����Ҫ������Ƿ�Ϊ�ն��󣨵��������������·�������ͻ����������⣩
	const auto& _itr = _spawnRoutePool.find(_spawnPointIdx);
	//��ָ��end()�����Ч��λ�ã��õ�����ָ���б�����һ��Ԫ�صĺ�һ��λ�ã���˵������������Ǵ����
	if (_itr == _spawnRoutePool.end())
	{
		//std::cout << "Invalid SpawnPointIdx\n";
		return;
	}
	//��ȡ����·��
	const Route& _route = _itr->second;

	//��ȡ�����Ŷ�Ӧ������·���ϵ���Ƭ����������б�
	const Route::TilePointList _tilePointList = _route.GetTilePointList();

	//�������Ӧ�������ɵ���λ�ã������Ϸ���ڵ�ʵ�����꣩
	_pos.x = _mapRect.x + _tilePointList[0].x * TILE_SIZE + TILE_SIZE / 2;
	_pos.y = _mapRect.y + _tilePointList[0].y * TILE_SIZE + TILE_SIZE / 2;

	//ʵ�����ù���ĳ�ʼλ�����н�·��
	_enemy->SetRoute(&_route);
	_enemy->SetPosition(_pos);
	#pragma endregion

	#pragma region SetSkillTrigger
	//���ù���ļ���
	_enemy->SetRecoverSkillTrigger(
		//����һ�������ͷ��ߵĲ���
		[&](Enemy* _src)
		{
			//��ȡ�ָ����ܵ�Ӱ��뾶�����Ϊ������˵���ù���û��������ܣ�ֱ�ӽ���
			double _radius = _src->GetSkillRecoverRadius();
			if (_radius < 0)
				return;

			//�����0����˵�����ƽ����Լ���Ч
			if (_radius == 0)
			{
				_src->IncreaseHealthBy(_src->GetSkillRecoverIntensity());
				return;
			}

			//Ϊ����԰����Լ����ڵķ�Χ�ڵ�����Ч�������ȡ�����ͷ��ߵ�λ��
			const Vector2& _srcPos = _src->GetPosition();
			//�������ϵ����е��ˣ�������Ƿ��ڼ���Ӱ��뾶��Χ��
			for (Enemy* _dst : enemyPool)
			{
				//��ȡǱ��Ŀ���λ��
				const Vector2& _dstPos = _dst->GetPosition();
				//�����ͷ�����õ��˼�ľ��루�������ȵļ���������֮ǰ��Vector2����д�õģ�
				double _distance = (_dstPos - _srcPos).Length();

				//���о�����жϣ����ڷ�Χ��������Ѫ��
				if (_distance <= _radius)
					_dst->IncreaseHealthBy(_src->GetSkillRecoverIntensity());
			}
		}
	);
	#pragma endregion
}

bool EnemyManager::IsEnemyCleaned() const
{
	return enemyPool.NoBusyEnemy();
}

EnemyPool& EnemyManager::GetEnemyPool()
{
	return enemyPool;
}

void EnemyManager::UpdateCollisionBullet()
{
	//�����������е��ˣ��������ӵ�����ײ���
	for (Enemy* _enemy : enemyPool)
	{
		//���õ�����������������ֹ������ؼ����������㣨������Դ��ڴ�ѭ����䣬�޷���ʱ���ⲿ�����Ƴ���
		if (!_enemy->IsAlive()) continue;

		//��ȡ����λ����ߴ�
		const Vector2& _enemyPosition = _enemy->GetPosition();
		const SDL_Point& _enemySize = _enemy->GetSize();

		//�������������ӵ�
		static const BulletManager::BulletList& _bulletList = BulletManager::Instance()->GetBulletList();
		for (Bullet* _bullet : _bulletList)
		{
			//�����ӵ��Ѳ��ܷ�����ײ�ж������������ӵ�
			if (!_bullet->CanCollide()) continue;
			//��ȡ�ӵ�������λ�ã���ʱ����ߴ磨���Ƿ�Χ�˺�����˵�������ӵ�������Χ�нϴ�հ�����������ײ�춨���Ӿ��ϲ�����
			const Vector2& _bulletPosition = _bullet->GetPosition();

			//����ӵ����ĵ��Ƿ��������ͼ���η����غ�
			if (_bulletPosition.x >= _enemyPosition.x - _enemySize.x / 2
				&& _bulletPosition.x <= _enemyPosition.x + _enemySize.x / 2
				&& _bulletPosition.y <= _enemyPosition.y + _enemySize.y / 2
				&& _bulletPosition.y >= _enemyPosition.y - _enemySize.y / 2)
			{
				//��ȡ�ӵ����˺����˺���Χ
				double _damage = _bullet->GetDamage();
				double _damageRadius = _bullet->GetDamageRadius();

				//����ΧΪ����������˵���ǵ����˺���ֱ�ӶԵ�������ӵ��ļ�ѪЧ�����ú���������������Լ�����ҵ��߼���
				if (_damageRadius <= 0)
					_enemy->DecreaseHealthBy(_damage);
				//������ɷ�Χ�˺�
				else
				{
					//���±������е��ˣ����Ѽ�ⷶΧ�ӵ�ǰ�����������󵽳������е���
					for (Enemy* _target : enemyPool)
					{
						if ((_target->GetPosition() - _bulletPosition).Length() <= _damageRadius)
							_target->DecreaseHealthBy(_damage);
					}
				}

				//�����ӵ�����ײЧ��
				_bullet->OnCollide(_enemy);
			}
		}
	}
}

void EnemyManager::UpdateCollisionHome()
{
	#pragma region GetHomePosition
	//��ȡ�����ڵ���Ƭ�����
	static const SDL_Point& _homePt = ProcessManager::Instance()->map.GetHomeIdx();
	//��ȡ��Ƭ��ͼ�������Ϸ���ڵ���ȾRect
	static const SDL_Rect& _mapRect = ProcessManager::Instance()->mapRect;
	//ͨ����������������������ҵ���ͼ�����ϽǶ����ʵ�����꣨Vector2��������ʵ������㣬��SDL_Point����ɢ�Ĵ���λ�ķ�ʵ������㣩
	static const Vector2 _homePos = {
		(double)(_mapRect.x + _homePt.x * TILE_SIZE),
		(double)(_mapRect.y + _homePt.y * TILE_SIZE),
	};
	#pragma endregion

	//���������ڳ��ĵ��ˣ�������Ƿ������ң������˾ʹ�������������յ����˺�
	for (Enemy* _enemy : enemyPool)
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
			ProcessManager::Instance()->DecreaseHealthBy(_enemy->GetAttackDamage());
		}
	}
}
