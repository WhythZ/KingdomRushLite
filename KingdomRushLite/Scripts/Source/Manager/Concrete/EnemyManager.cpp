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
	//更新对象池内的活跃对象
	enemyPool.OnUpdate(_delta);

	//碰撞检测
	UpdateCollisionBullet();
	UpdateCollisionHome();
}

void EnemyManager::OnRender(SDL_Renderer* _renderer)
{
	enemyPool.OnRender(_renderer);
}

void EnemyManager::SpawnEnemy(EnemyType _type, int _spawnPointIdx)
{
	//从池中获取怪物对象
	Enemy* _enemy = enemyPool.Acquire(_type);
	//若指定类型无效使得指针仍未指向实例化的对象，则停止生成
	if (_enemy == nullptr)
		return;

	#pragma region LocateSpawnPosition
	//临时用于存储位置信息
	static Vector2 _pos;
	//获取地图Rect用于定位
	static const SDL_Rect& _mapRect = ProcessManager::Instance()->mapRect;

	//获取生成路径池，用于索引具体的出生点
	static const RoutePool& _spawnRoutePool = ProcessManager::Instance()->map.GetSpawnRoutePool();
	//获取_spawnPointIdx在路径池中对应的对象，需要检测其是否为空对象（当输入的索引超出路径总数就会产生这个问题）
	const auto& _itr = _spawnRoutePool.find(_spawnPointIdx);
	//若指向end()这个无效的位置（该迭代器指向列表的最后一个元素的后一个位置）则说明传入的索引是错误的
	if (_itr == _spawnRoutePool.end())
	{
		//std::cout << "Invalid SpawnPointIdx\n";
		return;
	}
	//获取生成路径
	const Route& _route = _itr->second;

	//获取传入编号对应的生成路径上的瓦片坐标点索引列表
	const Route::TilePointList _tilePointList = _route.GetTilePointList();

	//计算怪物应当被生成到的位置（相对游戏窗口的实际坐标）
	_pos.x = _mapRect.x + _tilePointList[0].x * TILE_SIZE + TILE_SIZE / 2;
	_pos.y = _mapRect.y + _tilePointList[0].y * TILE_SIZE + TILE_SIZE / 2;

	//实际设置怪物的初始位置与行进路径
	_enemy->SetRoute(&_route);
	_enemy->SetPosition(_pos);
	#pragma endregion

	#pragma region SetSkillTrigger
	//设置怪物的技能
	_enemy->SetRecoverSkillTrigger(
		//接收一个技能释放者的参数
		[&](Enemy* _src)
		{
			//获取恢复技能的影响半径，如果为负，则说明该怪物没有这个技能，直接结束
			double _radius = _src->GetSkillRecoverRadius();
			if (_radius < 0)
				return;

			//如果是0，则说明治疗仅对自己生效
			if (_radius == 0)
			{
				_src->IncreaseHealthBy(_src->GetSkillRecoverIntensity());
				return;
			}

			//为正则对包括自己在内的范围内敌人生效，先需获取技能释放者的位置
			const Vector2& _srcPos = _src->GetPosition();
			//遍历场上的所有敌人，检测其是否在技能影响半径范围内
			for (Enemy* _dst : enemyPool)
			{
				//获取潜在目标的位置
				const Vector2& _dstPos = _dst->GetPosition();
				//计算释放者与该敌人间的距离（向量长度的计算是我们之前在Vector2类内写好的）
				double _distance = (_dstPos - _srcPos).Length();

				//进行距离的判断，若在范围内则增加血量
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
	//遍历场上所有敌人，进行与子弹的碰撞检测
	for (Enemy* _enemy : enemyPool)
	{
		//若该敌人已死则跳过，防止无意义地继续后续运算（因程序仍处于此循环语句，无法及时在外部将其移除）
		if (!_enemy->IsAlive()) continue;

		//获取敌人位置与尺寸
		const Vector2& _enemyPosition = _enemy->GetPosition();
		const SDL_Point& _enemySize = _enemy->GetSize();

		//遍历场上所有子弹
		static const BulletManager::BulletList& _bulletList = BulletManager::Instance()->GetBulletList();
		for (Bullet* _bullet : _bulletList)
		{
			//若该子弹已不能发生碰撞判定，则跳过该子弹
			if (!_bullet->CanCollide()) continue;
			//获取子弹的中心位置，暂时无需尺寸（若是范围伤害那另说），因子弹矩形外围有较大空白区域，用于碰撞检定在视觉上不合适
			const Vector2& _bulletPosition = _bullet->GetPosition();

			//检测子弹中心点是否与敌人贴图矩形发生重合
			if (_bulletPosition.x >= _enemyPosition.x - _enemySize.x / 2
				&& _bulletPosition.x <= _enemyPosition.x + _enemySize.x / 2
				&& _bulletPosition.y <= _enemyPosition.y + _enemySize.y / 2
				&& _bulletPosition.y >= _enemyPosition.y - _enemySize.y / 2)
			{
				//获取子弹的伤害与伤害范围
				double _damage = _bullet->GetDamage();
				double _damageRadius = _bullet->GetDamageRadius();

				//若范围为非正数，则说明是单体伤害，直接对敌人造成子弹的减血效果（该函数内置死亡检测以及爆金币等逻辑）
				if (_damageRadius <= 0)
					_enemy->DecreaseHealthBy(_damage);
				//否则造成范围伤害
				else
				{
					//重新遍历所有敌人，即把检测范围从当前单个敌人扩大到场上所有敌人
					for (Enemy* _target : enemyPool)
					{
						if ((_target->GetPosition() - _bulletPosition).Length() <= _damageRadius)
							_target->DecreaseHealthBy(_damage);
					}
				}

				//处理子弹的碰撞效果
				_bullet->OnCollide(_enemy);
			}
		}
	}
}

void EnemyManager::UpdateCollisionHome()
{
	#pragma region GetHomePosition
	//获取家所在的瓦片坐标点
	static const SDL_Point& _homePt = ProcessManager::Instance()->map.GetHomeIdx();
	//获取瓦片地图相对于游戏窗口的渲染Rect
	static const SDL_Rect& _mapRect = ProcessManager::Instance()->mapRect;
	//通过上述两个东西，计算出家的贴图的左上角顶点的实际坐标（Vector2是连续的实际坐标点，而SDL_Point是离散的带单位的非实际坐标点）
	static const Vector2 _homePos = {
		(double)(_mapRect.x + _homePt.x * TILE_SIZE),
		(double)(_mapRect.y + _homePt.y * TILE_SIZE),
	};
	#pragma endregion

	//遍历所有在场的敌人，检测其是否碰到家，碰到了就处死，并计算家收到的伤害
	for (Enemy* _enemy : enemyPool)
	{
		//已经死了的不算
		if (!_enemy->IsAlive()) continue;

		//获取敌人的位置（贴图的中心点的位置）
		const Vector2 _pos = _enemy->GetPosition();

		//如果敌人的中心点处于家的贴图（此处把贴图当作碰撞箱）
		if (_pos.x >= _homePos.x
			&& _pos.y >= _homePos.y
			&& _pos.x <= _homePos.x + TILE_SIZE
			&& _pos.y <= _homePos.y + TILE_SIZE)
		{
			//处死敌人
			_enemy->Kill();
			//计算伤害
			ProcessManager::Instance()->DecreaseHealthBy(_enemy->GetAttackDamage());
		}
	}
}
