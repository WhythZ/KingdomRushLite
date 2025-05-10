#include "../../../Header/Player/Concrete/PlayerDragon.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header/Manager/Concrete/AudioManager.h"
#include "../../../Header/Manager/Concrete/ConfigManager.h"
#include "../../../Header/Manager/Concrete/EnemyManager.h"
#include "../../../Header/Map/Tile.h"

PlayerDragon::PlayerDragon()
{
	//设定类型
	type = PlayerType::Dragon;

	//根据纹理图片实际尺寸设定
	size = { 96,96 };

	#pragma region SetAnimation
	//获取纹理
	static SDL_Texture* _playerDragonSpriteSheet = ResourceManager::Instance()->
		GetTexturePool().find(TextureResID::Player_Dragon)->second;

	static const std::vector<int> _idleIndiciesUp = { 4,5,6,7 };
	static const std::vector<int> _idleIndiciesDown = { 0,1,2,3 };
	static const std::vector<int> _idleIndiciesLeft = { 8,9,10,11 };
	static const std::vector<int> _idleIndiciesRight = { 12,13,14,15 };

	static const std::vector<int> _moveIndiciesUp = { 4,5,6,7 };
	static const std::vector<int> _moveIndiciesDown = { 0,1,2,3 };
	static const std::vector<int> _moveIndiciesLeft = { 8,9,10,11 };
	static const std::vector<int> _moveIndiciesRight = { 12,13,14,15 };

	static const std::vector<int> _skillIndiciesUp = { 20,21 };
	static const std::vector<int> _skillIndiciesDown = { 16,17 };
	static const std::vector<int> _skillIndiciesLeft = { 24,25 };
	static const std::vector<int> _skillIndiciesRight = { 28,29 };

	idleState->animUp.SetLoop(true); idleState->animUp.SetAnimFrames(_playerDragonSpriteSheet, 4, 8, _idleIndiciesUp);
	idleState->animDown.SetLoop(true); idleState->animDown.SetAnimFrames(_playerDragonSpriteSheet, 4, 8, _idleIndiciesDown);
	idleState->animLeft.SetLoop(true); idleState->animLeft.SetAnimFrames(_playerDragonSpriteSheet, 4, 8, _idleIndiciesLeft);
	idleState->animRight.SetLoop(true); idleState->animRight.SetAnimFrames(_playerDragonSpriteSheet, 4, 8, _idleIndiciesRight);

	moveState->animUp.SetLoop(true); moveState->animUp.SetAnimFrames(_playerDragonSpriteSheet, 4, 8, _moveIndiciesUp);
	moveState->animDown.SetLoop(true); moveState->animDown.SetAnimFrames(_playerDragonSpriteSheet, 4, 8, _moveIndiciesDown);
	moveState->animLeft.SetLoop(true); moveState->animLeft.SetAnimFrames(_playerDragonSpriteSheet, 4, 8, _moveIndiciesLeft);
	moveState->animRight.SetLoop(true); moveState->animRight.SetAnimFrames(_playerDragonSpriteSheet, 4, 8, _moveIndiciesRight);

	skillState->animUp.SetLoop(false); skillState->animUp.SetAnimFrames(_playerDragonSpriteSheet, 4, 8, _skillIndiciesUp);
	skillState->animDown.SetLoop(false); skillState->animDown.SetAnimFrames(_playerDragonSpriteSheet, 4, 8, _skillIndiciesDown);
	skillState->animLeft.SetLoop(false); skillState->animLeft.SetAnimFrames(_playerDragonSpriteSheet, 4, 8, _skillIndiciesLeft);
	skillState->animRight.SetLoop(false); skillState->animRight.SetAnimFrames(_playerDragonSpriteSheet, 4, 8, _skillIndiciesRight);
	#pragma endregion

	#pragma region SetStats
	//获取配置数据
	static const ConfigManager::PlayerConfigPrefab& _configPrefab = ConfigManager::Instance()->playerDragonPrefab;

	speed = _configPrefab.speed * TILE_SIZE;
	skill00Cooldown = _configPrefab.skill00Cooldown;
	skill00Damage = _configPrefab.skill00Damage;
	skill01Cooldown = _configPrefab.skill01Cooldown;
	skill01Damage = _configPrefab.skill01Damage;
	#pragma endregion

	#pragma region SetSkillTimer
	//由于该冷却值在子类中被获取，故需要在子类构造函数中设置
	skill00Timer.SetWaitTime(skill00Cooldown);
	skill01Timer.SetWaitTime(skill01Cooldown);
	#pragma endregion

	#pragma region SetSkillAnimation
	#pragma region SetFlashSkillAnimation
	//每个方向的技能特效纹理单独使用一张SpriteSheet，都有5帧
	static const std::vector<int> _flashSkillIndiciesUp = { 0,1,2,3,4 };
	static const std::vector<int> _flashSkillIndiciesDown = { 4,3,2,1,0 };
	static const std::vector<int> _flashSkillIndiciesLeft = { 4,3,2,1,0 };
	static const std::vector<int> _flashSkillIndiciesRight = { 0,1,2,3,4 };

	//设置动画帧纹理图片
	static SDL_Texture* _flashSkillUpSS = ResourceManager::Instance()->
		GetTexturePool().find(TextureResID::Player_Dragon_Skill_Flash_Up)->second;
	flashSkillAnimUp.SetLoop(false); flashSkillAnimUp.SetAnimFrames(_flashSkillUpSS, 5, 1, _flashSkillIndiciesUp);
	static SDL_Texture* _flashSkillDownSS = ResourceManager::Instance()->
		GetTexturePool().find(TextureResID::Player_Dragon_Skill_Flash_Down)->second;
	flashSkillAnimDown.SetLoop(false); flashSkillAnimDown.SetAnimFrames(_flashSkillDownSS, 5, 1, _flashSkillIndiciesDown);
	static SDL_Texture* _flashSkillLeftSS = ResourceManager::Instance()->
		GetTexturePool().find(TextureResID::Player_Dragon_Skill_Flash_Left)->second;
	flashSkillAnimLeft.SetLoop(false); flashSkillAnimLeft.SetAnimFrames(_flashSkillLeftSS, 1, 5, _flashSkillIndiciesLeft);
	static SDL_Texture* _flashSkillRightSS = ResourceManager::Instance()->
		GetTexturePool().find(TextureResID::Player_Dragon_Skill_Flash_Right)->second;
	flashSkillAnimRight.SetLoop(false); flashSkillAnimRight.SetAnimFrames(_flashSkillRightSS, 1, 5, _flashSkillIndiciesRight);

	//设置回调函数
	flashSkillAnimUp.SetOnAnimFinished(
		[&]()
		{
			isReleaseFlashSkill = false;
			flashSkillAnimUp.Restart();
			stateMachine->ChangeState(idleState);
		}
	);
	flashSkillAnimDown.SetOnAnimFinished(
		[&]()
		{
			isReleaseFlashSkill = false;
			flashSkillAnimDown.Restart();
			stateMachine->ChangeState(idleState);
		}
	);
	flashSkillAnimLeft.SetOnAnimFinished(
		[&]()
		{
			isReleaseFlashSkill = false;
			flashSkillAnimLeft.Restart();
			stateMachine->ChangeState(idleState);
		}
	);
	flashSkillAnimRight.SetOnAnimFinished(
		[&]()
		{
			isReleaseFlashSkill = false;
			flashSkillAnimRight.Restart();
			stateMachine->ChangeState(idleState);
		}
	);

	//初始化动画指针
	flashSkillAnimCurrent = &flashSkillAnimLeft;
	#pragma endregion

	#pragma region SetFlashSkillAnimation
	static const std::vector<int> _impactSkillIndiciesUp = { 0,1,2,3,4 };
	static const std::vector<int> _impactSkillIndiciesDown = { 4,3,2,1,0 };
	static const std::vector<int> _impactSkillIndiciesLeft = { 4,3,2,1,0 };
	static const std::vector<int> _impactSkillIndiciesRight = { 0,1,2,3,4 };

	static SDL_Texture* _impactSkillUpSS = ResourceManager::Instance()->
		GetTexturePool().find(TextureResID::Player_Dragon_Skill_Impact_Up)->second;
	impactSkillAnimUp.SetLoop(false); impactSkillAnimUp.SetAnimFrames(_impactSkillUpSS, 5, 1, _impactSkillIndiciesUp);
	static SDL_Texture* _impactSkillDownSS = ResourceManager::Instance()->
		GetTexturePool().find(TextureResID::Player_Dragon_Skill_Impact_Down)->second;
	impactSkillAnimDown.SetLoop(false); impactSkillAnimDown.SetAnimFrames(_impactSkillDownSS, 5, 1, _impactSkillIndiciesDown);
	static SDL_Texture* _impactSkillLeftSS = ResourceManager::Instance()->
		GetTexturePool().find(TextureResID::Player_Dragon_Skill_Impact_Left)->second;
	impactSkillAnimLeft.SetLoop(false); impactSkillAnimLeft.SetAnimFrames(_impactSkillLeftSS, 1, 5, _impactSkillIndiciesLeft);
	static SDL_Texture* _impactSkillRightSS = ResourceManager::Instance()->
		GetTexturePool().find(TextureResID::Player_Dragon_Skill_Impact_Right)->second;
	impactSkillAnimRight.SetLoop(false); impactSkillAnimRight.SetAnimFrames(_impactSkillRightSS, 1, 5, _impactSkillIndiciesRight);

	impactSkillAnimUp.SetOnAnimFinished(
		[&]()
		{
			isReleaseImpactSkill = false;
			impactSkillAnimUp.Restart();
			stateMachine->ChangeState(idleState);
		}
	);
	impactSkillAnimDown.SetOnAnimFinished(
		[&]()
		{
			isReleaseImpactSkill = false;
			impactSkillAnimDown.Restart();
			stateMachine->ChangeState(idleState);
		}
	);
	impactSkillAnimLeft.SetOnAnimFinished(
		[&]()
		{
			isReleaseImpactSkill = false;
			impactSkillAnimLeft.Restart();
			stateMachine->ChangeState(idleState);
		}
	);
	impactSkillAnimRight.SetOnAnimFinished(
		[&]()
		{
			isReleaseImpactSkill = false;
			impactSkillAnimRight.Restart();
			stateMachine->ChangeState(idleState);
		}
	);

	impactSkillAnimCurrent = &impactSkillAnimLeft;
	#pragma endregion
	#pragma endregion
}

void PlayerDragon::OnUpdate(double _delta)
{
	Player::OnUpdate(_delta);

	//处理技能数据，注意此处计算技能特效纹理图片（同时也是碰撞体积）的左上顶点的方法之差异
	if (isReleaseFlashSkill)
	{
		switch (facingDir)
		{
		case FacingDir::Up:
			flashSkillAnimCurrent = &flashSkillAnimUp;
			flashSkillAnimRect.w = flashSkillSize.y;
			flashSkillAnimRect.h = flashSkillSize.x;
			flashSkillAnimRect.x = (int)(position.x - flashSkillSize.y / 2);
			flashSkillAnimRect.y = (int)(position.y - size.y / 2 - flashSkillSize.x);

			//此处将碰撞检测范围向玩家动画扩大了一些，使得攻击判定范围比用于动画渲染的Rect更大，手感更好
			flashSkillCollideRect.w = flashSkillAnimRect.w;
			flashSkillCollideRect.h = flashSkillAnimRect.h + (int)(size.y / 2);
			flashSkillCollideRect.x = flashSkillAnimRect.x;
			flashSkillCollideRect.y = flashSkillAnimRect.y;
			break;
		case FacingDir::Down:
			flashSkillAnimCurrent = &flashSkillAnimDown;
			flashSkillAnimRect.w = flashSkillSize.y;
			flashSkillAnimRect.h = flashSkillSize.x;
			flashSkillAnimRect.x = (int)(position.x - flashSkillSize.y / 2);
			flashSkillAnimRect.y = (int)(position.y + size.y / 2);

			flashSkillCollideRect.w = flashSkillAnimRect.w;
			flashSkillCollideRect.h = flashSkillAnimRect.h + (int)(size.y / 2);
			flashSkillCollideRect.x = flashSkillAnimRect.x;
			flashSkillCollideRect.y = flashSkillAnimRect.y - (int)(size.y / 2);
			break;
		case FacingDir::Left:
			flashSkillAnimCurrent = &flashSkillAnimLeft;
			flashSkillAnimRect.w = flashSkillSize.x;
			flashSkillAnimRect.h = flashSkillSize.y;
			flashSkillAnimRect.x = (int)(position.x - size.x / 2 - flashSkillSize.x);
			flashSkillAnimRect.y = (int)(position.y - flashSkillSize.y / 2);

			flashSkillCollideRect.w = flashSkillAnimRect.w + (int)(size.x / 2);
			flashSkillCollideRect.h = flashSkillAnimRect.h;
			flashSkillCollideRect.x = flashSkillAnimRect.x;
			flashSkillCollideRect.y = flashSkillAnimRect.y;
			break;
		case FacingDir::Right:
			flashSkillAnimCurrent = &flashSkillAnimRight;
			flashSkillAnimRect.w = flashSkillSize.x;
			flashSkillAnimRect.h = flashSkillSize.y;
			flashSkillAnimRect.x = (int)(position.x + size.x / 2);
			flashSkillAnimRect.y = (int)(position.y - flashSkillSize.y / 2);

			flashSkillCollideRect.w = flashSkillAnimRect.w + (int)(size.x / 2);
			flashSkillCollideRect.h = flashSkillAnimRect.h;
			flashSkillCollideRect.x = flashSkillAnimRect.x - (int)(size.x / 2);
			flashSkillCollideRect.y = flashSkillAnimRect.y;
			break;
		default:
			break;
		}
		flashSkillAnimCurrent->OnUpdate(_delta);
	}
	if (isReleaseImpactSkill)
	{
		switch (facingDir)
		{
		case FacingDir::Up:
			impactSkillAnimCurrent = &impactSkillAnimUp;
			impactSkillAnimRect.w = impactSkillSize.y;
			impactSkillAnimRect.h = impactSkillSize.x;
			impactSkillAnimRect.x = (int)(position.x - impactSkillSize.y / 2);
			impactSkillAnimRect.y = (int)(position.y - size.y / 2 - impactSkillSize.x);

			impactSkillCollideRect.w = impactSkillAnimRect.w;
			impactSkillCollideRect.h = impactSkillAnimRect.h + (int)(size.y / 2);
			impactSkillCollideRect.x = impactSkillAnimRect.x;
			impactSkillCollideRect.y = impactSkillAnimRect.y;
			break;
		case FacingDir::Down:
			impactSkillAnimCurrent = &impactSkillAnimDown;
			impactSkillAnimRect.w = impactSkillSize.y;
			impactSkillAnimRect.h = impactSkillSize.x;
			impactSkillAnimRect.x = (int)(position.x - impactSkillSize.y / 2);
			impactSkillAnimRect.y = (int)(position.y + size.y / 2);

			impactSkillCollideRect.w = impactSkillAnimRect.w;
			impactSkillCollideRect.h = impactSkillAnimRect.h + (int)(size.y / 2);
			impactSkillCollideRect.x = impactSkillAnimRect.x;
			impactSkillCollideRect.y = impactSkillAnimRect.y - (int)(size.y / 2);
			break;
		case FacingDir::Left:
			impactSkillAnimCurrent = &impactSkillAnimLeft;
			impactSkillAnimRect.w = impactSkillSize.x;
			impactSkillAnimRect.h = impactSkillSize.y;
			impactSkillAnimRect.x = (int)(position.x - size.x / 2 - impactSkillSize.x);
			impactSkillAnimRect.y = (int)(position.y - impactSkillSize.y / 2);

			impactSkillCollideRect.w = impactSkillAnimRect.w + (int)(size.x / 2);
			impactSkillCollideRect.h = impactSkillAnimRect.h;
			impactSkillCollideRect.x = impactSkillAnimRect.x;
			impactSkillCollideRect.y = impactSkillAnimRect.y;
			break;
		case FacingDir::Right:
			impactSkillAnimCurrent = &impactSkillAnimRight;
			impactSkillAnimRect.w = impactSkillSize.x;
			impactSkillAnimRect.h = impactSkillSize.y;
			impactSkillAnimRect.x = (int)(position.x + size.x / 2);
			impactSkillAnimRect.y = (int)(position.y - impactSkillSize.y / 2);

			impactSkillCollideRect.w = impactSkillAnimRect.w + (int)(size.x / 2);
			impactSkillCollideRect.h = impactSkillAnimRect.h;
			impactSkillCollideRect.x = impactSkillAnimRect.x - (int)(size.x / 2);
			impactSkillCollideRect.y = impactSkillAnimRect.y;
			break;
		default:
			break;
		}
		impactSkillAnimCurrent->OnUpdate(_delta);
	}

	//技能Rect更新后执行技能的碰撞
	if (canFlashSkillCollide)
	{
		canFlashSkillCollide = false;
		OnFlashCollide();
	}
	if (canImpactSkillCollide)
	{
		canImpactSkillCollide = false;
		OnImpactCollide();
	}
}

void PlayerDragon::OnRender(SDL_Renderer* _renderer)
{
	Player::OnRender(_renderer);

	//渲染技能特效
	static SDL_Point _point;
	if (isReleaseFlashSkill)
	{
		_point.x = flashSkillAnimRect.x; _point.y = flashSkillAnimRect.y;
		flashSkillAnimCurrent->OnRender(_renderer, _point);
	}
	else if (isReleaseImpactSkill)
	{
		_point.x = impactSkillAnimRect.x; _point.y = impactSkillAnimRect.y;
		impactSkillAnimCurrent->OnRender(_renderer, _point);
	}
}

void PlayerDragon::TryReleaseSkill00()
{
	if (!isSkill && canReleaseSkill00)
	{
		isReleaseFlashSkill = true;
		stateMachine->ChangeState(skillState);

		//释放技能音效
		AudioManager::Instance()->PlaySFX(SoundResID::Player_Dragon_Skill_Flash);
		//允许碰撞，然后等到Rect数据更新后执行碰撞
		canFlashSkillCollide = true;

		//技能进入冷却
		canReleaseSkill00 = false;
		skill00Timer.Restart();
	}
}

void PlayerDragon::TryReleaseSkill01()
{
	if (!isSkill && canReleaseSkill01)
	{
		isReleaseImpactSkill = true;
		stateMachine->ChangeState(skillState);

		//释放技能效果
		AudioManager::Instance()->PlaySFX(SoundResID::Player_Dragon_Skill_Impact);
		//允许碰撞，然后等到Rect数据更新后执行碰撞
		canImpactSkillCollide = true;

		//技能进入冷却
		canReleaseSkill01 = false;
		skill01Timer.Restart();
	}
}

void PlayerDragon::OnFlashCollide()
{
	//std::cout << "FlashSkillCollideRect=(" << flashSkillCollideRect.x << "," << flashSkillCollideRect.y << ","
	//	<< flashSkillCollideRect.w << "," << flashSkillCollideRect.h << ",)\n";

	//遍历场上所有敌人，进行与技能范围的碰撞检测
	for (Enemy* _enemy : EnemyManager::Instance()->GetEnemyPool())
	{
		//若该敌人已死则跳过，防止无意义地继续后续运算（因程序仍处于此循环语句，无法及时在外部将其移除）
		if (!_enemy->IsAlive()) continue;

		//获取敌人中心点位置，判断其是否处于技能范围Rect内
		const Vector2& _enemyPosition = _enemy->GetPosition();
		if (_enemyPosition.x >= flashSkillCollideRect.x
			&& _enemyPosition.x <= flashSkillCollideRect.x + flashSkillCollideRect.w
			&& _enemyPosition.y >= flashSkillCollideRect.y
			&& _enemyPosition.y <= flashSkillCollideRect.y + flashSkillCollideRect.h)
		{
			//造成伤害，该函数内置死亡检测以及掉落物概率生成逻辑
			_enemy->DecreaseHealthBy(skill00Damage);

			//std::cout << "Flash Hit\n";
		}
	}
}

void PlayerDragon::OnImpactCollide()
{
	//std::cout << "ImpactSkillCollideRect=(" << impactSkillCollideRect.x << "," << impactSkillCollideRect.y << ","
	//	<< impactSkillCollideRect.w << "," << impactSkillCollideRect.h << ",)\n";

	for (Enemy* _enemy : EnemyManager::Instance()->GetEnemyPool())
	{
		if (!_enemy->IsAlive()) continue;

		const Vector2& _enemyPosition = _enemy->GetPosition();
		if (_enemyPosition.x >= impactSkillCollideRect.x
			&& _enemyPosition.x <= impactSkillCollideRect.x + impactSkillCollideRect.w
			&& _enemyPosition.y >= impactSkillCollideRect.y
			&& _enemyPosition.y <= impactSkillCollideRect.y + impactSkillCollideRect.h)
		{
			_enemy->DecreaseHealthBy(skill01Damage);
			//造成伤害的同时，将敌人眩晕（暂停其移动，但不能传入1，那会使得速度完全为0导致动画异常）
			_enemy->SlowDownBy(0.99, impactSkillSlowDownDuration);

			//std::cout << "Impact Hit\n";
		}
	}
}