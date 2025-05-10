#include "../../../Header/Infra/ObjectPool/EnemyPool.h"

void EnemyPool::OnUpdate(double _delta)
{
	//���¶�����ڵĻ�Ծ����
	RemoveDeadEnemies();

	for (Slime* _enemy : slimePool.GetBusyObjects())
		_enemy->OnUpdate(_delta);
	for (SlimeKing* _enemy : slimeKingPool.GetBusyObjects())
		_enemy->OnUpdate(_delta);
	for (Skeleton* _enemy : skeletonPool.GetBusyObjects())
		_enemy->OnUpdate(_delta);
	for (Goblin* _enemy : goblinPool.GetBusyObjects())
		_enemy->OnUpdate(_delta);
	for (GoblinPriest* _enemy : goblinPriestPool.GetBusyObjects())
		_enemy->OnUpdate(_delta);
}

void EnemyPool::OnRender(SDL_Renderer* _renderer)
{
	for (Slime* _enemy : slimePool.GetBusyObjects())
		_enemy->OnRender(_renderer);
	for (SlimeKing* _enemy : slimeKingPool.GetBusyObjects())
		_enemy->OnRender(_renderer);
	for (Skeleton* _enemy : skeletonPool.GetBusyObjects())
		_enemy->OnRender(_renderer);
	for (Goblin* _enemy : goblinPool.GetBusyObjects())
		_enemy->OnRender(_renderer);
	for (GoblinPriest* _enemy : goblinPriestPool.GetBusyObjects())
		_enemy->OnRender(_renderer);
}

EnemyPoolIterator EnemyPool::begin()
{
	std::vector<Enemy*> _allEnemies;

	//������æ������ӽ�һ��std::vector
	for (Slime* _e : slimePool.GetBusyObjects()) _allEnemies.emplace_back(_e);
	for (SlimeKing* _e : slimeKingPool.GetBusyObjects()) _allEnemies.emplace_back(_e);
	for (Skeleton* _e : skeletonPool.GetBusyObjects()) _allEnemies.emplace_back(_e);
	for (Goblin* _e : goblinPool.GetBusyObjects()) _allEnemies.emplace_back(_e);
	for (GoblinPriest* _e : goblinPriestPool.GetBusyObjects()) _allEnemies.emplace_back(_e);

	return EnemyPoolIterator(_allEnemies, 0);
}

EnemyPoolIterator EnemyPool::end()
{
	std::vector<Enemy*> _allEnemies;

	//��begin��ͬ
	for (Slime* _e : slimePool.GetBusyObjects()) _allEnemies.emplace_back(_e);
	for (SlimeKing* _e : slimeKingPool.GetBusyObjects()) _allEnemies.emplace_back(_e);
	for (Skeleton* _e : skeletonPool.GetBusyObjects()) _allEnemies.emplace_back(_e);
	for (Goblin* _e : goblinPool.GetBusyObjects()) _allEnemies.emplace_back(_e);
	for (GoblinPriest* _e : goblinPriestPool.GetBusyObjects()) _allEnemies.emplace_back(_e);

	//ע�������ǹ���end������������size��index
	return EnemyPoolIterator(_allEnemies, _allEnemies.size());
}

Enemy* EnemyPool::Acquire(EnemyType _type)
{
	switch (_type)
	{
	case EnemyType::None:
		return nullptr;
		break;
	case EnemyType::Slime:
		return slimePool.AcquireObject();
		break;
	case EnemyType::SlimeKing:
		return slimeKingPool.AcquireObject();
		break;
	case EnemyType::Skeleton:
		return skeletonPool.AcquireObject();
		break;
	case EnemyType::Goblin:
		return goblinPool.AcquireObject();
		break;
	case EnemyType::GoblinPriest:
		return goblinPriestPool.AcquireObject();
		break;
	default:
		return nullptr;
		break;
	}
}

bool EnemyPool::NoBusyEnemy() const
{
	if (slimePool.GetBusyObjects().empty()
		&& slimeKingPool.GetBusyObjects().empty()
		&& skeletonPool.GetBusyObjects().empty()
		&& goblinPool.GetBusyObjects().empty()
		&& goblinPriestPool.GetBusyObjects().empty())
		return true;
	return false;
}

void EnemyPool::RemoveDeadEnemies()
{
	//�������е��˳أ��ͷ������ĵ��˻ص�����
	for (Slime* _enemy : slimePool.GetBusyObjects())
	{
		if (!_enemy->IsAlive())
			slimePool.ReleaseObject(_enemy);
	}
	for (SlimeKing* _enemy : slimeKingPool.GetBusyObjects())
	{
		if (!_enemy->IsAlive())
			slimeKingPool.ReleaseObject(_enemy);
	}
	for (Skeleton* _enemy : skeletonPool.GetBusyObjects())
	{
		if (!_enemy->IsAlive())
			skeletonPool.ReleaseObject(_enemy);
	}
	for (Goblin* _enemy : goblinPool.GetBusyObjects())
	{
		if (!_enemy->IsAlive())
			goblinPool.ReleaseObject(_enemy);
	}
	for (GoblinPriest* _enemy : goblinPriestPool.GetBusyObjects())
	{
		if (!_enemy->IsAlive())
			goblinPriestPool.ReleaseObject(_enemy);
	}
}