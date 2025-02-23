#include "../../../Header/Enemy/Concrete/Goblin.h"
#include "../../../Header/Manager/Concrete/ConfigManager.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"

Goblin::Goblin()
{
	//设置怪物类型
	type = EnemyType::Goblin;

	//获取配置数据
	static const ConfigManager::EnemyConfigPrefab& _configPrefab = ConfigManager::Instance()->goblinPrefab;
	//获取纹理
	static SDL_Texture* _sprite = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Enemy_Goblin)->second;
	static SDL_Texture* _spriteSketch = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Enemy_Goblin_Sketch)->second;

	#pragma region SetAnimation
	static const std::vector<int> _spriteIdxListDown = { 0,1,2,3,4 };
	static const std::vector<int> _spriteIdxListUp = { 5,6,7,8,9 };
	static const std::vector<int> _spriteIdxListRight = { 10,11,12,13,14 };
	static const std::vector<int> _spriteIdxListLeft = { 15,16,17,18,19 };

	//设置上下左右的动画贴图
	animDown.SetLoop(true); animDown.SetAnimFrames(_sprite, 5, 4, _spriteIdxListDown);
	animUp.SetLoop(true); animUp.SetAnimFrames(_sprite, 5, 4, _spriteIdxListUp);
	animRight.SetLoop(true); animRight.SetAnimFrames(_sprite, 5, 4, _spriteIdxListRight);
	animLeft.SetLoop(true); animLeft.SetAnimFrames(_sprite, 5, 4, _spriteIdxListLeft);

	//设置上下左右的受击剪影贴图
	animDownSketch.SetLoop(true); animDownSketch.SetAnimFrames(_spriteSketch, 5, 4, _spriteIdxListDown);
	animUpSketch.SetLoop(true); animUpSketch.SetAnimFrames(_spriteSketch, 5, 4, _spriteIdxListUp);
	animRightSketch.SetLoop(true); animRightSketch.SetAnimFrames(_spriteSketch, 5, 4, _spriteIdxListRight);
	animLeftSketch.SetLoop(true); animLeftSketch.SetAnimFrames(_spriteSketch, 5, 4, _spriteIdxListLeft);
	#pragma endregion

	#pragma region BasicStats
	//设置基本的数值属性
	healthMaximum = _configPrefab.hp;
	healthCurrent = healthMaximum;

	speedMaximum = _configPrefab.speed;
	speedCurrent = speedMaximum;

	attackDamage = _configPrefab.damage;

	coinRatio = _configPrefab.coinRatio;
	#pragma endregion

	#pragma region Skill
	//设置技能相关数值属性
	skillRecoverCooldown = _configPrefab.recoverCooldown;
	skillRecoverRadius = _configPrefab.recoverRadius * TILE_SIZE;
	skillRecoverIntensity = _configPrefab.recoverIntensity;

	//设置冷却时间
	skillRecoverCooldowndTimer.SetWaitTime(skillRecoverCooldown);
	#pragma endregion

	//尺寸和瓦片大小保持一致
	spriteSize.x = TILE_SIZE;
	spriteSize.y = TILE_SIZE;
}