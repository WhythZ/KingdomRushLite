#include "../../../Header/Enemy/Concrete/Slime.h"
#include "../../../Header/Manager/Concrete/ConfigManager.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"

Slime::Slime()
{
	//设置怪物类型
	type = EnemyType::Slime;

	//尺寸和瓦片大小保持一致
	size.x = TILE_SIZE;
	size.y = TILE_SIZE;

	//获取配置数据
	static const ConfigManager::EnemyConfigPrefab& _configPrefab = ConfigManager::Instance()->slimePrefab;

	#pragma region SetAnimation
	//获取纹理
	static SDL_Texture* _sprites = ResourceManager::Instance()->
		GetTexturePool().find(TextureResID::Enemy_Slime)->second;
	static SDL_Texture* _spritesSketch = ResourceManager::Instance()->
		GetTexturePool().find(TextureResID::Enemy_Slime_Sketch)->second;

	static const std::vector<int> _spriteIdxListDown = { 0,1,2,3,4,5 };
	static const std::vector<int> _spriteIdxListUp = { 6,7,8,9,10,11 };
	static const std::vector<int> _spriteIdxListRight = { 12,13,14,15,16,17 };
	static const std::vector<int> _spriteIdxListLeft = { 18,19,20,21,22,23 };

	//设置上下左右的动画贴图
	animDown.SetLoop(true); animDown.SetAnimFrames(_sprites, 6, 4, _spriteIdxListDown);
	animUp.SetLoop(true); animUp.SetAnimFrames(_sprites, 6, 4, _spriteIdxListUp);
	animRight.SetLoop(true); animRight.SetAnimFrames(_sprites, 6, 4, _spriteIdxListRight);
	animLeft.SetLoop(true); animLeft.SetAnimFrames(_sprites, 6, 4, _spriteIdxListLeft);

	//设置上下左右的受击剪影贴图
	animDownSketch.SetLoop(true); animDownSketch.SetAnimFrames(_spritesSketch, 6, 4, _spriteIdxListDown);
	animUpSketch.SetLoop(true); animUpSketch.SetAnimFrames(_spritesSketch, 6, 4, _spriteIdxListUp);
	animRightSketch.SetLoop(true); animRightSketch.SetAnimFrames(_spritesSketch, 6, 4, _spriteIdxListRight);
	animLeftSketch.SetLoop(true); animLeftSketch.SetAnimFrames(_spritesSketch, 6, 4, _spriteIdxListLeft);
	#pragma endregion

	#pragma region SetBasicStats
	//设置基本的数值属性
	healthMaximum = _configPrefab.health;
	healthCurrent = healthMaximum;

	speedMaximum = _configPrefab.speed;
	speedCurrent = speedMaximum;

	attackDamage = _configPrefab.damage;

	coinRatio = _configPrefab.coinRatio;
	#pragma endregion

	#pragma region SetSkillStats
	//设置技能相关数值属性
	skillRecoverCooldown = _configPrefab.skillRecoverCooldown;
	skillRecoverRadius = _configPrefab.skillRecoverRadius * TILE_SIZE;
	skillRecoverIntensity = _configPrefab.skillRecoverIntensity;

	//设置冷却时间
	skillRecoverCooldownTimer.SetWaitTime(skillRecoverCooldown);
	#pragma endregion
}
