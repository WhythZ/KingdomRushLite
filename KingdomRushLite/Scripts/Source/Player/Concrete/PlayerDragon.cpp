#include "../../../Header/Player/Concrete/PlayerDragon.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header/Manager/Concrete/ConfigManager.h"
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
			flashSkillRect.w = flashSkillSize.y;
			flashSkillRect.h = flashSkillSize.x;
			flashSkillRect.x = (int)(position.x - flashSkillSize.y / 2);
			flashSkillRect.y = (int)(position.y - size.y / 2 - flashSkillSize.x);
			break;
		case FacingDir::Down:
			flashSkillAnimCurrent = &flashSkillAnimDown;
			flashSkillRect.w = flashSkillSize.y;
			flashSkillRect.h = flashSkillSize.x;
			flashSkillRect.x = (int)(position.x - flashSkillSize.y / 2);
			flashSkillRect.y = (int)(position.y + size.y / 2);
			break;
		case FacingDir::Left:
			flashSkillAnimCurrent = &flashSkillAnimLeft;
			flashSkillRect.w = flashSkillSize.x;
			flashSkillRect.h = flashSkillSize.y;
			flashSkillRect.x = (int)(position.x - size.x / 2 - flashSkillSize.x);
			flashSkillRect.y = (int)(position.y - flashSkillSize.y / 2);
			break;
		case FacingDir::Right:
			flashSkillAnimCurrent = &flashSkillAnimRight;
			flashSkillRect.w = flashSkillSize.x;
			flashSkillRect.h = flashSkillSize.y;
			flashSkillRect.x = (int)(position.x + size.x / 2);
			flashSkillRect.y = (int)(position.y - flashSkillSize.y / 2);
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
			impactSkillRect.w = impactSkillSize.y;
			impactSkillRect.h = impactSkillSize.x;
			impactSkillRect.x = (int)(position.x - impactSkillSize.y / 2);
			impactSkillRect.y = (int)(position.y - size.y / 2 - impactSkillSize.x);
			break;
		case FacingDir::Down:
			impactSkillAnimCurrent = &impactSkillAnimDown;
			impactSkillRect.w = impactSkillSize.y;
			impactSkillRect.h = impactSkillSize.x;
			impactSkillRect.x = (int)(position.x - impactSkillSize.y / 2);
			impactSkillRect.y = (int)(position.y + size.y / 2);
			break;
		case FacingDir::Left:
			impactSkillAnimCurrent = &impactSkillAnimLeft;
			impactSkillRect.w = impactSkillSize.x;
			impactSkillRect.h = impactSkillSize.y;
			impactSkillRect.x = (int)(position.x - size.x / 2 - impactSkillSize.x);
			impactSkillRect.y = (int)(position.y - impactSkillSize.y / 2);
			break;
		case FacingDir::Right:
			impactSkillAnimCurrent = &impactSkillAnimRight;
			impactSkillRect.w = impactSkillSize.x;
			impactSkillRect.h = impactSkillSize.y;
			impactSkillRect.x = (int)(position.x + size.x / 2);
			impactSkillRect.y = (int)(position.y - impactSkillSize.y / 2);
			break;
		default:
			break;
		}
		impactSkillAnimCurrent->OnUpdate(_delta);
	}
}

void PlayerDragon::OnRender(SDL_Renderer* _renderer)
{
	Player::OnRender(_renderer);

	//渲染技能特效
	static SDL_Point _point;
	if (isReleaseFlashSkill)
	{
		_point.x = flashSkillRect.x; _point.y = flashSkillRect.y;
		flashSkillAnimCurrent->OnRender(_renderer, _point);
	}
	else if (isReleaseImpactSkill)
	{
		_point.x = impactSkillRect.x; _point.y = impactSkillRect.y;
		impactSkillAnimCurrent->OnRender(_renderer, _point);
	}
}

void PlayerDragon::TryReleaseSkill00()
{
	if (!isSkill && canReleaseSkill00)
	{
		isReleaseFlashSkill = true;
		stateMachine->ChangeState(skillState);

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

		//技能进入冷却
		canReleaseSkill01 = false;
		skill01Timer.Restart();
	}
}