#include "../../../Header/Manager/Concrete/EnemyManager.h"
#include "../../../Header/Manager/Concrete/ProcessManager.h"
#include "../../../Header/Manager/Concrete/BulletManager.h"
#include "../../../Header/Enemy/Concrete/Slime.h"
#include "../../../Header/Enemy/Concrete/SlimeKing.h"
#include "../../../Header/Enemy/Concrete/Skeleton.h"
#include "../../../Header/Enemy/Concrete/Goblin.h"
#include "../../../Header/Enemy/Concrete/GoblinPriest.h"

EnemyManager::~EnemyManager()
{
	for (Enemy* _enemy : enemyList)
		delete _enemy;
}

void EnemyManager::OnUpdate(double _delta)
{
	for (Enemy* _enemy : enemyList)
		_enemy->OnUpdate(_delta);

	//碰撞检测
	UpdateCollisionBullet();
	UpdateCollisionHome();

	//检测死亡敌人并将其移除
	RemoveDeadEnemies();

	//if (enemyList.size() > 0)
	//{
	//	std::cout << "FirstEnemyPos=" << enemyList[0]->GetPosition() << ", Target=" << enemyList[0]->GetTargetPosition() << ", ";
	//	std::cout << "Velocity=" << enemyList[0]->GetVelocity() << "\n";
	//}
}

void EnemyManager::OnRender(SDL_Renderer* _renderer)
{
	for (Enemy* _enemy : enemyList)
		_enemy->OnRender(_renderer);
}

void EnemyManager::SpawnEnemy(EnemyType _type, int _spawnPointIdx)
{
	#pragma region Instantiate
	//生成怪物对象
	Enemy* _enemy = nullptr;
	//确定怪物类型
	switch (_type)
	{
	case EnemyType::Slime:
		_enemy = new Slime();
		break;
	case EnemyType::SlimeKing:
		_enemy = new SlimeKing();
		break;
	case EnemyType::Skeleton:
		_enemy = new Skeleton();
		break;
	case EnemyType::Goblin:
		_enemy = new Goblin();
		break;
	case EnemyType::GoblinPriest:
		_enemy = new GoblinPriest();
		break;
	default:
		break;
	}
	//若指定类型无效使得指针仍未指向实例化的对象，则停止生成
	if (_enemy == nullptr)
		return;
	#pragma endregion

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
			//获取恢复技能的影响半径，如果非正，那就说明该怪物没有这个技能，直接结束
			double _radius = _src->GetSkillRecoverRadius();
			if (_radius <= 0) return;

			//获取技能释放者的位置
			const Vector2& _srcPos = _src->GetPosition();
			//遍历场上的所有敌人，检测其是否在技能影响半径范围内
			for (Enemy* _dst : enemyList)
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

	//将怪物添加到统计列表
	enemyList.push_back(_enemy);

	//std::cout << "_tilePointList.size()=" << _tilePointList.size() << "\n";
	//std::cout << "SpawnEnemy=" << _type << ", SpawnPositon=" << _pos << ", Route=" << _route << "\n";
}

bool EnemyManager::IsEnemyCleaned() const
{
	return enemyList.empty();
}

const EnemyManager::EnemyList& EnemyManager::GetEnemyList() const
{
	return enemyList;
}

void EnemyManager::UpdateCollisionBullet()
{
	//遍历场上所有敌人，进行与子弹的碰撞检测
	for (Enemy* _enemy : enemyList)
	{
		//若该敌人已死则跳过，防止无意义地继续后续运算（因程序仍处于此循环语句，无法及时在外部将其移除）
		if (!_enemy->IsAlive()) continue;

		//获取敌人位置与尺寸
		const Vector2& _enemyPosition = _enemy->GetPosition();
		const Vector2& _enemySize = _enemy->GetSize();

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

				//若范围为非正数，则说明是单体伤害
				if (_damageRadius <= 0)
				{
					//处理敌人被子弹碰撞后的减血效果（该函数内置死亡检测以及爆金币等逻辑）
					_enemy->DecreaseHealthBy(_damage);
				}
				//否则造成范围伤害
				else
				{
					//重新遍历所有敌人，即把检测范围从当前单个敌人扩大到场上所有敌人
					for (Enemy* _target : enemyList)
					{
						if ((_target->GetPosition() - _bulletPosition).Length() <= _damageRadius)
						{
							//处理敌人被子弹碰撞后的减血效果（该函数内置死亡检测以及爆金币等逻辑）
							_target->DecreaseHealthBy(_damage);
						}
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
	for (Enemy* _enemy : enemyList)
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

void EnemyManager::RemoveDeadEnemies()
{
	//函数remove_if遍历列表，按照Lambda的返回的bool，将true的元素统统放入enemyList列表容器的末尾，并将返回一个指向第一个true的元素的迭代器
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

	//删除所有死亡的敌人，此时的enemyList在remove_if的排列下，所有死亡的敌人指针均在列表末尾
	enemyList.erase(_begin, enemyList.end());
}
