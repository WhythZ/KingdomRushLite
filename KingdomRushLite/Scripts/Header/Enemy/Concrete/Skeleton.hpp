#ifndef _SKELETON_HPP_
#define _SKELETON_HPP_

#include "../Enemy.hpp"
#include "../EnemyType.hpp"
#include "../../Manager/ConfigManager.hpp"
#include "../../Manager/ResourceManager.hpp"

class Skeleton :public Enemy
{
public:
	Skeleton();
	~Skeleton() = default;
};

Skeleton::Skeleton()
{
	//设置怪物类型
	type = EnemyType::Skeleton;

	//获取配置数据
	static const ConfigManager::EnemyConfigPrefab& _configPrefab = ConfigManager::Instance()->skeletonPrefab;
	//获取纹理
	static SDL_Texture* _sprite = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Enemy_Skeleton)->second;
	static SDL_Texture* _spriteSketch = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Enemy_Skeleton_Sketch)->second;

	#pragma region SetAnimation
	static const std::vector<int> _spriteIdxListDown = { 0,1,2,3,4,5 };
	static const std::vector<int> _spriteIdxListUp = { 6,7,8,9,10,11 };
	static const std::vector<int> _spriteIdxListRight = { 12,13,14,15,16,17 };
	static const std::vector<int> _spriteIdxListLeft = { 18,19,20,21,22,23 };

	//设置上下左右的动画贴图
	animDown.SetLoop(true); animDown.SetAnimFrames(_sprite, 6, 4, _spriteIdxListDown);
	animUp.SetLoop(true); animUp.SetAnimFrames(_sprite, 6, 4, _spriteIdxListUp);
	animRight.SetLoop(true); animRight.SetAnimFrames(_sprite, 6, 4, _spriteIdxListRight);
	animLeft.SetLoop(true); animLeft.SetAnimFrames(_sprite, 6, 4, _spriteIdxListLeft);

	//设置上下左右的受击剪影贴图
	animDownSketch.SetLoop(true); animDownSketch.SetAnimFrames(_spriteSketch, 6, 4, _spriteIdxListDown);
	animUpSketch.SetLoop(true); animUpSketch.SetAnimFrames(_spriteSketch, 6, 4, _spriteIdxListUp);
	animRightSketch.SetLoop(true); animRightSketch.SetAnimFrames(_spriteSketch, 6, 4, _spriteIdxListRight);
	animLeftSketch.SetLoop(true); animLeftSketch.SetAnimFrames(_spriteSketch, 6, 4, _spriteIdxListLeft);
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

	#pragma region SkillStats
	//设置技能相关数值属性
	skillRecoverCooldown = _configPrefab.recoverCooldown;
	skillRecoverRange = _configPrefab.recoverRange;
	skillRecoverIntensity = _configPrefab.recoverIntensity;
	#pragma endregion

	//尺寸和瓦片大小保持一致
	spriteSize.x = TILE_SIZE;
	spriteSize.y = TILE_SIZE;
}

#endif
