#include "../../../Header/Manager/Concrete/EnemyManager.h"
#include "../../../Header/Manager/Concrete/ProcessManager.h"
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

	//��ײ���
	ProcessCollisionBullet();
	ProcessCollisionHome();

	//����������˲������Ƴ�
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
	//���ɹ������
	Enemy* _enemy = nullptr;
	//ȷ����������
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
	//��ָ��������Чʹ��ָ����δָ��ʵ�����Ķ�����ֹͣ����
	if (_enemy == nullptr)
		return;
	#pragma endregion

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

	//���ù���ļ���
	_enemy->SetRecoverSkillTrigger(
		//����һ�������ͷ��ߵĲ���
		[&](Enemy* _src)
		{
			//��ȡ�ָ����ܵ�Ӱ��뾶������������Ǿ�˵���ù���û��������ܣ�ֱ�ӽ���
			double _radius = _src->GetSkillRecoverRadius();
			if (_radius <= 0) return;

			//��ȡ�����ͷ��ߵ�λ��
			const Vector2& _srcPos = _src->GetPosition();
			//�������ϵ����е��ˣ�������Ƿ��ڼ���Ӱ��뾶��Χ��
			for (Enemy* _dst : enemyList)
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

	//��������ӵ�ͳ���б�
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

void EnemyManager::ProcessCollisionBullet()
{
	#pragma region GetHomePosition
	//��ȡ�����ڵ���Ƭ�����
	static const SDL_Point& _homePt = ProcessManager::Instance()->map.GetHomePoint();
	//��ȡ��Ƭ��ͼ�������Ϸ���ڵ���ȾRect
	static const SDL_Rect& _mapRect = ProcessManager::Instance()->mapRect;
	//ͨ����������������������ҵ���ͼ�����ϽǶ����ʵ�����꣨Vector2��������ʵ������㣬��SDL_Point����ɢ�Ĵ���λ�ķ�ʵ������㣩
	static const Vector2 _homePos = {
		(double)(_mapRect.x + _homePt.x * TILE_SIZE),
		(double)(_mapRect.y + _homePt.y * TILE_SIZE),
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
			ProcessManager::Instance()->DecreaseHealthBy(_enemy->GetAttackDamage());
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
