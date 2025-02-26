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
	static SDL_Texture* _spriteSheet = ResourceManager::Instance()->
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

	idleState->animUp.SetLoop(true); idleState->animUp.SetAnimFrames(_spriteSheet, 4, 8, _idleIndiciesUp);
	idleState->animDown.SetLoop(true); idleState->animDown.SetAnimFrames(_spriteSheet, 4, 8, _idleIndiciesDown);
	idleState->animLeft.SetLoop(true); idleState->animLeft.SetAnimFrames(_spriteSheet, 4, 8, _idleIndiciesLeft);
	idleState->animRight.SetLoop(true); idleState->animRight.SetAnimFrames(_spriteSheet, 4, 8, _idleIndiciesRight);

	moveState->animUp.SetLoop(true); moveState->animUp.SetAnimFrames(_spriteSheet, 4, 8, _moveIndiciesUp);
	moveState->animDown.SetLoop(true); moveState->animDown.SetAnimFrames(_spriteSheet, 4, 8, _moveIndiciesDown);
	moveState->animLeft.SetLoop(true); moveState->animLeft.SetAnimFrames(_spriteSheet, 4, 8, _moveIndiciesLeft);
	moveState->animRight.SetLoop(true); moveState->animRight.SetAnimFrames(_spriteSheet, 4, 8, _moveIndiciesRight);

	skillState->animUp.SetLoop(false); skillState->animUp.SetAnimFrames(_spriteSheet, 4, 8, _skillIndiciesUp);
	skillState->animDown.SetLoop(false); skillState->animDown.SetAnimFrames(_spriteSheet, 4, 8, _skillIndiciesDown);
	skillState->animLeft.SetLoop(false); skillState->animLeft.SetAnimFrames(_spriteSheet, 4, 8, _skillIndiciesLeft);
	skillState->animRight.SetLoop(false); skillState->animRight.SetAnimFrames(_spriteSheet, 4, 8, _skillIndiciesRight);
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
}

void PlayerDragon::ReleaseSkill00()
{

}

void PlayerDragon::ReleaseSkill01()
{

}