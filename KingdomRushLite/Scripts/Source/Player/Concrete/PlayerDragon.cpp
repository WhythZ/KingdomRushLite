#include "../../../Header/Player/Concrete/PlayerDragon.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header/Manager/Concrete/AudioManager.h"
#include "../../../Header/Manager/Concrete/ConfigManager.h"
#include "../../../Header/Manager/Concrete/EnemyManager.h"
#include "../../../Header/Map/Tile.h"

PlayerDragon::PlayerDragon()
{
	//�趨����
	type = PlayerType::Dragon;

	//��������ͼƬʵ�ʳߴ��趨
	size = { 96,96 };

	#pragma region SetAnimation
	//��ȡ����
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
	//��ȡ��������
	static const ConfigManager::PlayerConfigPrefab& _configPrefab = ConfigManager::Instance()->playerDragonPrefab;

	speed = _configPrefab.speed * TILE_SIZE;
	skill00Cooldown = _configPrefab.skill00Cooldown;
	skill00Damage = _configPrefab.skill00Damage;
	skill01Cooldown = _configPrefab.skill01Cooldown;
	skill01Damage = _configPrefab.skill01Damage;
	#pragma endregion

	#pragma region SetSkillTimer
	//���ڸ���ȴֵ�������б���ȡ������Ҫ�����๹�캯��������
	skill00Timer.SetWaitTime(skill00Cooldown);
	skill01Timer.SetWaitTime(skill01Cooldown);
	#pragma endregion

	#pragma region SetSkillAnimation
	#pragma region SetFlashSkillAnimation
	//ÿ������ļ�����Ч������ʹ��һ��SpriteSheet������5֡
	static const std::vector<int> _flashSkillIndiciesUp = { 0,1,2,3,4 };
	static const std::vector<int> _flashSkillIndiciesDown = { 4,3,2,1,0 };
	static const std::vector<int> _flashSkillIndiciesLeft = { 4,3,2,1,0 };
	static const std::vector<int> _flashSkillIndiciesRight = { 0,1,2,3,4 };

	//���ö���֡����ͼƬ
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

	//���ûص�����
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

	//��ʼ������ָ��
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

	//���������ݣ�ע��˴����㼼����Ч����ͼƬ��ͬʱҲ����ײ����������϶���ķ���֮����
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

			//�˴�����ײ��ⷶΧ����Ҷ���������һЩ��ʹ�ù����ж���Χ�����ڶ�����Ⱦ��Rect�����ָи���
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

	//����Rect���º�ִ�м��ܵ���ײ
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

	//��Ⱦ������Ч
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

		//�ͷż�����Ч
		AudioManager::Instance()->PlaySFX(SoundResID::Player_Dragon_Skill_Flash);
		//������ײ��Ȼ��ȵ�Rect���ݸ��º�ִ����ײ
		canFlashSkillCollide = true;

		//���ܽ�����ȴ
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

		//�ͷż���Ч��
		AudioManager::Instance()->PlaySFX(SoundResID::Player_Dragon_Skill_Impact);
		//������ײ��Ȼ��ȵ�Rect���ݸ��º�ִ����ײ
		canImpactSkillCollide = true;

		//���ܽ�����ȴ
		canReleaseSkill01 = false;
		skill01Timer.Restart();
	}
}

void PlayerDragon::OnFlashCollide()
{
	//std::cout << "FlashSkillCollideRect=(" << flashSkillCollideRect.x << "," << flashSkillCollideRect.y << ","
	//	<< flashSkillCollideRect.w << "," << flashSkillCollideRect.h << ",)\n";

	//�����������е��ˣ������뼼�ܷ�Χ����ײ���
	for (Enemy* _enemy : EnemyManager::Instance()->GetEnemyPool())
	{
		//���õ�����������������ֹ������ؼ����������㣨������Դ��ڴ�ѭ����䣬�޷���ʱ���ⲿ�����Ƴ���
		if (!_enemy->IsAlive()) continue;

		//��ȡ�������ĵ�λ�ã��ж����Ƿ��ڼ��ܷ�ΧRect��
		const Vector2& _enemyPosition = _enemy->GetPosition();
		if (_enemyPosition.x >= flashSkillCollideRect.x
			&& _enemyPosition.x <= flashSkillCollideRect.x + flashSkillCollideRect.w
			&& _enemyPosition.y >= flashSkillCollideRect.y
			&& _enemyPosition.y <= flashSkillCollideRect.y + flashSkillCollideRect.h)
		{
			//����˺����ú���������������Լ���������������߼�
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
			//����˺���ͬʱ��������ѣ�Σ���ͣ���ƶ��������ܴ���1���ǻ�ʹ���ٶ���ȫΪ0���¶����쳣��
			_enemy->SlowDownBy(0.99, impactSkillSlowDownDuration);

			//std::cout << "Impact Hit\n";
		}
	}
}