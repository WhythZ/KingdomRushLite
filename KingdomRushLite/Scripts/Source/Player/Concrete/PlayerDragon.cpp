#include "../../../Header/Player/Concrete/PlayerDragon.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header/Manager/Concrete/ConfigManager.h"

PlayerDragon::PlayerDragon()
{
	type = PlayerType::Dragon;

	//获取配置数据
	static const ConfigManager::PlayerConfigPrefab& _configPrefab = ConfigManager::Instance()->playerDragonPrefab;

	#pragma region SetAnimation
	//获取纹理
	static SDL_Texture* _spriteSheet = ResourceManager::Instance()->
		GetTexturePool().find(TextureResID::Player_Dragon)->second;

	static const std::vector<int> _idleIndiciesUp = { 4,5,6,7 };
	static const std::vector<int> _idleIndiciesDown = { 0,1,2,3 };
	static const std::vector<int> _idleIndiciesLeft = { 12,13,14,15 };
	static const std::vector<int> _idleIndiciesRight = { 8,9,10,11 };

	static const std::vector<int> _moveIndiciesUp = { 4,5,6,7 };
	static const std::vector<int> _moveIndiciesDown = { 0,1,2,3 };
	static const std::vector<int> _moveIndiciesLeft = { 12,13,14,15 };
	static const std::vector<int> _moveIndiciesRight = { 8,9,10,11 };

	static const std::vector<int> _skillIndiciesUp = { 20,21 };
	static const std::vector<int> _skillIndiciesDown = { 16,17 };
	static const std::vector<int> _skillIndiciesLeft = { 28,29 };
	static const std::vector<int> _skillIndiciesRight = { 24,25 };

	idleState->animUp.SetLoop(true); idleState->animUp.SetAnimFrames(_spriteSheet, 5, 4, _idleIndiciesUp);
	idleState->animDown.SetLoop(true); idleState->animDown.SetAnimFrames(_spriteSheet, 5, 4, _idleIndiciesDown);
	idleState->animLeft.SetLoop(true); idleState->animLeft.SetAnimFrames(_spriteSheet, 5, 4, _idleIndiciesLeft);
	idleState->animRight.SetLoop(true); idleState->animRight.SetAnimFrames(_spriteSheet, 5, 4, _idleIndiciesRight);

	moveState->animUp.SetLoop(true); moveState->animUp.SetAnimFrames(_spriteSheet, 5, 4, _moveIndiciesUp);
	moveState->animDown.SetLoop(true); moveState->animDown.SetAnimFrames(_spriteSheet, 5, 4, _moveIndiciesDown);
	moveState->animLeft.SetLoop(true); moveState->animLeft.SetAnimFrames(_spriteSheet, 5, 4, _moveIndiciesLeft);
	moveState->animRight.SetLoop(true); moveState->animRight.SetAnimFrames(_spriteSheet, 5, 4, _moveIndiciesRight);

	skillState->animUp.SetLoop(false); skillState->animUp.SetAnimFrames(_spriteSheet, 5, 4, _skillIndiciesUp);
	skillState->animDown.SetLoop(false); skillState->animDown.SetAnimFrames(_spriteSheet, 5, 4, _skillIndiciesDown);
	skillState->animLeft.SetLoop(false); skillState->animLeft.SetAnimFrames(_spriteSheet, 5, 4, _skillIndiciesLeft);
	skillState->animRight.SetLoop(false); skillState->animRight.SetAnimFrames(_spriteSheet, 5, 4, _skillIndiciesRight);
	#pragma endregion
}

void PlayerDragon::ReleaseSkill00()
{

}

void PlayerDragon::ReleaseSkill01()
{

}