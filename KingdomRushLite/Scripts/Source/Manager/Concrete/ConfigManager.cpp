#include "../../../Header/Manager/Concrete/ConfigManager.h"
#include "../../../Header/Manager/Concrete/ProcessManager.h"

bool ConfigManager::LoadConfig(const std::string& _path)
{
	#pragma region GetJsonRoot
	//�Ӵ���·����ȡ�ļ������ж��Ƿ��ȡʧ��
	std::ifstream _file(_path);
	if (!_file.good()) return false;

	//���ļ�����һ���Զ�ȡ������Ȼ��ر��ļ�
	std::stringstream _strStream;
	_strStream << _file.rdbuf();
	_file.close();

	//��_strStream������ת��ΪC����ַ������������洢��cJSON_Parse�����ǿ��ٵ������ģ��ʺ���Ҫ��_jsonRoot�������٣�������Ƿ�����ɹ�
	cJSON* _jsonRoot = cJSON_Parse(_strStream.str().c_str());
	if (!_jsonRoot) return false;

	//���ͼ��
	if (_jsonRoot->type != cJSON_Object) return false;
	#pragma endregion

	//������������
	if (!ParseBasicConfigPrefab(basicPrefab, cJSON_GetObjectItem(_jsonRoot, "basic"))) return false;

	//�����������
	if (!ParsePlayerConfigPrefab(playerDragonPrefab, cJSON_GetObjectItem(_jsonRoot, "player_dragon"))) return false;

	//����������������
	cJSON* _jsonTower = cJSON_GetObjectItem(_jsonRoot, "tower");
	if (!ParseTowerConfigPrefab(archerPrefab, cJSON_GetObjectItem(_jsonTower, "archer"))) return false;
	if (!ParseTowerConfigPrefab(axemanPrefab, cJSON_GetObjectItem(_jsonTower, "axeman"))) return false;
	if (!ParseTowerConfigPrefab(gunnerPrefab, cJSON_GetObjectItem(_jsonTower, "gunner"))) return false;

	//��������������
	cJSON* _jsonEnemyType = cJSON_GetObjectItem(_jsonRoot, "enemy");
	if (!ParseEnemyConfigPrefab(slimePrefab, cJSON_GetObjectItem(_jsonEnemyType, "slime"))) return false;
	if (!ParseEnemyConfigPrefab(slimeKingPrefab, cJSON_GetObjectItem(_jsonEnemyType, "slime_king"))) return false;
	if (!ParseEnemyConfigPrefab(skeletonPrefab, cJSON_GetObjectItem(_jsonEnemyType, "skeleton"))) return false;
	if (!ParseEnemyConfigPrefab(goblinPrefab, cJSON_GetObjectItem(_jsonEnemyType, "goblin"))) return false;
	if (!ParseEnemyConfigPrefab(goblinPriestPrefab, cJSON_GetObjectItem(_jsonEnemyType, "goblin_priest"))) return false;

	//�ͷ��ڴ沢����
	cJSON_Delete(_jsonRoot);
	return true;
}

bool ConfigManager::LoadWaves(const std::string& _path)
{
	#pragma region GetJsonRoot
	//�Ӵ���·����ȡ�ļ������ж��Ƿ��ȡʧ��
	std::ifstream _file(_path);
	if (!_file.good()) return false;

	//���ļ�����һ���Զ�ȡ������Ȼ��ر��ļ�
	std::stringstream _strStream;
	_strStream << _file.rdbuf();
	_file.close();

	//��_strStream������ת��ΪC����ַ������������洢��cJSON_Parse�����ǿ��ٵ������ģ��ʺ���Ҫ��_jsonRoot�������٣�������Ƿ�����ɹ�
	cJSON* _jsonRoot = cJSON_Parse(_strStream.str().c_str());
	if (!_jsonRoot) return false;

	//�ؿ����ɶ�����Σ�Wave�������ɵ����飬����Ҫ���ļ��ı�������ʽ�ϣ�������������/�������Ƕ��󣩵ļ�⣬�����޷����к����Ľ���
	if (_jsonRoot->type != cJSON_Array)
	{
		//���ٺ��ٽ�����������ֹ�ڴ�й©
		cJSON_Delete(_jsonRoot);
		return false;
	}
	#pragma endregion

	//����ָ��ͬ���ε�json����Ŀɸ���ָ��
	cJSON* _jsonWave = nullptr;
	//json��������ĵ���������ǰ�߱������ߣ����飩�е�json����Ԫ��
	cJSON_ArrayForEach(_jsonWave, _jsonRoot)
	{
		//���õ��Ķ�������ͽ����жϣ��˴���Ҫ�ǻ����ŵ�json���������Ǿ������˶���
		if (_jsonWave->type != cJSON_Object) continue;

		//�õ������������б�β���Ŀ�Ԫ�أ�׼������¼���ѭ����ȡ���Ĳ�����Ϣ
		waveList.emplace_back();
		Wave& _wave = waveList.back();

		//�Ե�һ��ֵ��ӦΪdouble���֣����м��
		cJSON* _jsonRewards = cJSON_GetObjectItem(_jsonWave, "coin_rewards");
		if (_jsonRewards && _jsonRewards->type == cJSON_Number)
			_wave.coinRewards = _jsonRewards->valuedouble;
		//�Եڶ���ֵ��ӦΪdouble���֣����м��
		cJSON* _jsonWaveInterval = cJSON_GetObjectItem(_jsonWave, "wave_interval");
		if (_jsonWaveInterval && _jsonWaveInterval->type == cJSON_Number)
			_wave.interval = _jsonWaveInterval->valuedouble;
		//�Ե�����ֵ��ӦΪ�����¼������飩
		cJSON* _jsonSpawnList = cJSON_GetObjectItem(_jsonWave, "spawn_list");
		if (_jsonSpawnList && _jsonSpawnList->type == cJSON_Array)
		{
			//�����������еĶ���
			cJSON* _jsonSpawnEvent = nullptr;
			cJSON_ArrayForEach(_jsonSpawnEvent, _jsonSpawnList)
			{
				//����õ��Ķ��������
				if (_jsonWave->type != cJSON_Object) continue;

				//�õ�_wave.spawnEventListĩβ�����Ŀ�Ԫ�أ�׼�������������¼��
				_wave.spawnEventList.emplace_back();
				Wave::SpawnEvent& _spawnEvent = _wave.spawnEventList.back();

				//����һ��ֵ��ӦΪdouble��
				cJSON* _jsonEventInterval = cJSON_GetObjectItem(_jsonSpawnEvent, "event_interval");
				if (_jsonEventInterval && _jsonEventInterval->type == cJSON_Number)
					_spawnEvent.interval = _jsonEventInterval->valuedouble;
				//���ڶ���ֵ��ӦΪint��
				cJSON* _jsonSpawnPoint = cJSON_GetObjectItem(_jsonSpawnEvent, "spawn_point");
				if (_jsonSpawnPoint && _jsonSpawnPoint->type == cJSON_Number)
					_spawnEvent.spawnPoint = _jsonSpawnPoint->valueint;
				//��������ֵ��ӦΪstring��
				cJSON* _jsonEnemyType = cJSON_GetObjectItem(_jsonSpawnEvent, "enemy_type");
				if (_jsonEnemyType && _jsonEnemyType->type == cJSON_String)
				{
					//_jsonEnemyType->valuestring��C����ַ������ַ�ָ�룬����ֱ����==�����еȣ�����ʵ�ʱȽϵ��ǵ�ַ������ʹ��strcmp�������бȽ�
					if (strcmp(_jsonEnemyType->valuestring, "slime") == 0) { _spawnEvent.enemyType = EnemyType::Slime; }
					else if (strcmp(_jsonEnemyType->valuestring, "slime_king") == 0) { _spawnEvent.enemyType = EnemyType::SlimeKing; }
					else if (strcmp(_jsonEnemyType->valuestring, "skeleton") == 0) { _spawnEvent.enemyType = EnemyType::Skeleton; }
					else if (strcmp(_jsonEnemyType->valuestring, "goblin") == 0) { _spawnEvent.enemyType = EnemyType::Goblin; }
					else if (strcmp(_jsonEnemyType->valuestring, "goblin_priest") == 0) { _spawnEvent.enemyType = EnemyType::GoblinPriest; }
				}
			}
		}

		//�����������ֵ�е����һ���������¼��б�Ϊ�գ���˵����_wave��ȡʧ�ܣ�ǰ���߶���Ĭ��ֵ����ȡʧ�ܾͲ�ȡĬ��ֵ������Ҫ����waveList�е���
		if (_wave.spawnEventList.empty())
			waveList.pop_back();
	}

	//��ֹ�ڴ�й©����Ϊ_jsonRoot�ǿ����ڶ����ģ���_jsonWave�ǶԺ��ߵĲ��ֿ�����������ϵ����ע������˳�򣩣�������Ҫ�Զ��߽�������
	cJSON_Delete(_jsonWave);
	cJSON_Delete(_jsonRoot);

	//���waveList�Ƿ�Ϊ�գ�Ϊ����˵�������ļ���д����
	if (waveList.empty()) return false;
	//һ�ж�ûë���Ļ��������շ��سɹ����ź�
	return true;
}

bool ConfigManager::LoadMap(const std::string& _path)
{
	return ProcessManager::Instance()->map.Load(_path);
}

bool ConfigManager::ParseBasicConfigPrefab(BasicConfigPrefab& _prefab, cJSON* _jsonRoot)
{
	//��⴫������
	if (!_jsonRoot || _jsonRoot->type != cJSON_Object)
		return false;

	cJSON* _jsonWindowWidth = cJSON_GetObjectItem(_jsonRoot, "window_width");
	cJSON* _jsonWindowHeight = cJSON_GetObjectItem(_jsonRoot, "window_height");
	cJSON* _jsonWindowTitle = cJSON_GetObjectItem(_jsonRoot, "window_title");

	if (!_jsonWindowWidth || !_jsonWindowHeight || !_jsonWindowTitle
		|| _jsonWindowWidth->type != cJSON_Number
		|| _jsonWindowHeight->type != cJSON_Number
		|| _jsonWindowTitle->type != cJSON_String)
	{
		return false;
	}

	_prefab.windowWidth = _jsonWindowWidth->valueint;
	_prefab.windowHeight = _jsonWindowHeight->valueint;
	_prefab.windowTitle = _jsonWindowTitle->valuestring;

	return true;
}

bool ConfigManager::ParsePlayerConfigPrefab(PlayerConfigPrefab& _prefab, cJSON* _jsonRoot)
{
	//��⴫������
	if (!_jsonRoot || _jsonRoot->type != cJSON_Object)
		return false;

	cJSON* _jsonSpeed = cJSON_GetObjectItem(_jsonRoot, "speed");
	if (!_jsonSpeed || _jsonSpeed->type != cJSON_Number) return false;
	_prefab.speed = _jsonSpeed->valuedouble;

	cJSON* _jsonNormalAtkCd = cJSON_GetObjectItem(_jsonRoot, "skill_00_cooldown");
	if (!_jsonNormalAtkCd || _jsonNormalAtkCd->type != cJSON_Number) return false;
	_prefab.skill00Cooldown = _jsonNormalAtkCd->valuedouble;

	cJSON* _jsonNormalAtkDmg = cJSON_GetObjectItem(_jsonRoot, "skill_00_damage");
	if (!_jsonNormalAtkDmg || _jsonNormalAtkDmg->type != cJSON_Number) return false;
	_prefab.skill00Damage = _jsonNormalAtkDmg->valuedouble;

	cJSON* _jsonSkillCd = cJSON_GetObjectItem(_jsonRoot, "skill_01_cooldown");
	if (!_jsonSkillCd || _jsonSkillCd->type != cJSON_Number) return false;
	_prefab.skill01Cooldown = _jsonSkillCd->valuedouble;

	cJSON* _jsonSkillDmg = cJSON_GetObjectItem(_jsonRoot, "skill_01_damage");
	if (!_jsonSkillDmg || _jsonSkillDmg->type != cJSON_Number) return false;
	_prefab.skill01Damage = _jsonSkillDmg->valuedouble;

	return true;
}

bool ConfigManager::ParseTowerConfigPrefab(TowerConfigPrefab& _prefab, cJSON* _jsonRoot)
{
	//��⴫������
	if (!_jsonRoot || _jsonRoot->type != cJSON_Object)
		return false;

	cJSON* _jsonCooldown = cJSON_GetObjectItem(_jsonRoot, "cooldown");
	if (!ParseNumberArray(_prefab.cooldown, TOWER_MAX_LEVEL, _jsonCooldown)) return false;

	cJSON* _jsonDamage = cJSON_GetObjectItem(_jsonRoot, "damage");
	if (!ParseNumberArray(_prefab.damage, TOWER_MAX_LEVEL, _jsonDamage)) return false;

	cJSON* _jsonFireRadius = cJSON_GetObjectItem(_jsonRoot, "fire_radius");
	if (!ParseNumberArray(_prefab.fireRadius, TOWER_MAX_LEVEL, _jsonFireRadius)) return false;

	cJSON* _jsonBuildCost = cJSON_GetObjectItem(_jsonRoot, "build_cost");
	if (!ParseNumberArray(_prefab.buildCost, TOWER_MAX_LEVEL, _jsonBuildCost)) return false;

	cJSON* _jsonUpgradeCost = cJSON_GetObjectItem(_jsonRoot, "upgrade_cost");
	//ע��˴�����ĳ�����(TOWER_MAX_LEVEL - 1)����Ϊ�������ĳ�ʼ�ȼ���1�����Կ�������������ߵȼ���1
	if (!ParseNumberArray(_prefab.upgradeCost, TOWER_MAX_LEVEL - 1, _jsonUpgradeCost)) return false;

	return true;
}

bool ConfigManager::ParseEnemyConfigPrefab(EnemyConfigPrefab& _prefab, cJSON* _jsonRoot)
{
	//��⴫������
	if (!_jsonRoot || _jsonRoot->type != cJSON_Object)
		return false;

	cJSON* _jsonHealth = cJSON_GetObjectItem(_jsonRoot, "health");
	if (!_jsonHealth || _jsonHealth->type != cJSON_Number) return false;
	_prefab.health = _jsonHealth->valuedouble;

	cJSON* _jsonSpeed = cJSON_GetObjectItem(_jsonRoot, "speed");
	if (!_jsonSpeed || _jsonSpeed->type != cJSON_Number) return false;
	_prefab.speed = _jsonSpeed->valuedouble;

	cJSON* _jsonDamage = cJSON_GetObjectItem(_jsonRoot, "damage");
	if (!_jsonDamage || _jsonDamage->type != cJSON_Number) return false;
	_prefab.damage = _jsonDamage->valuedouble;

	cJSON* _jsonCoinRatio = cJSON_GetObjectItem(_jsonRoot, "coin_ratio");
	if (!_jsonCoinRatio || _jsonCoinRatio->type != cJSON_Number) return false;
	_prefab.coinRatio = _jsonCoinRatio->valuedouble;

	cJSON* _jsonSkillRecoverCooldown = cJSON_GetObjectItem(_jsonRoot, "skill_recover_cooldown");
	if (!_jsonSkillRecoverCooldown || _jsonSkillRecoverCooldown->type != cJSON_Number) return false;
	_prefab.skillRecoverCooldown = _jsonSkillRecoverCooldown->valuedouble;

	cJSON* _jsonSkillRecoverRadius = cJSON_GetObjectItem(_jsonRoot, "skill_recover_radius");
	if (!_jsonSkillRecoverRadius || _jsonSkillRecoverRadius->type != cJSON_Number) return false;
	_prefab.skillRecoverRadius = _jsonSkillRecoverRadius->valuedouble;

	cJSON* _jsonSkillRecoveIntensity = cJSON_GetObjectItem(_jsonRoot, "skill_recover_intensity");
	if (!_jsonSkillRecoveIntensity || _jsonSkillRecoveIntensity->type != cJSON_Number) return false;
	_prefab.skillRecoverIntensity = _jsonSkillRecoveIntensity->valuedouble;

	return true;
}

bool ConfigManager::ParseNumberArray(double* _arr, int _maxLength, cJSON* _jsonRoot)
{
	//����Ƿ�Ϊ��������
	if (!_jsonRoot || _jsonRoot->type != cJSON_Array)
		return false;

	//���ڼ�¼ѭ�����ִΣ�ʹ�����ǿ���ͨ��������������Ԫ��
	int _idx = -1;
	cJSON* _jsonElem = nullptr;
	cJSON_ArrayForEach(_jsonElem, _jsonRoot)
	{
		//��������
		_idx++;

		//����⵽�������ͣ���˵�������ļ���ë��
		if (_jsonElem->type != cJSON_Number)
			return false;

		//_maxLength�������ǣ����������ļ��еĸ�����洢��20���������ȼ�����ֵ��������ֻ��Ҫǰ10��
		if (_idx >= _maxLength)
			continue;

		//��ȡ����
		_arr[_idx] = _jsonElem->valuedouble;
	}

	return true;
}
