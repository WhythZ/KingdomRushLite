#ifndef _SLIME_HPP_
#define _SLIME_HPP_

#include "../Enemy.hpp"
#include "../EnemyType.hpp"
#include "../../Manager/ConfigManager.hpp"
#include "../../Manager/ResourceManager.hpp"

class Slime :public Enemy
{
public:
	Slime();
	~Slime() = default;

private:

};

Slime::Slime()
{
	//记录静态的常量纹理池
	static const TexturePool& _spritePool = ResourceManager::GetInstance()->GetTexturePool();
	//获取配置数据
	static const ConfigManager::EnemyConfigPrefab& _configPrefab = ConfigManager::GetInstance()->slimePrefab;
	//获取纹理
	static const SDL_Texture* _sprite = _spritePool.find(SpriteResID::Enemy_Slime)->second;
	static const SDL_Texture* _spriteSketch = _spritePool.find(SpriteResID::Enemy_Slime_Sketch)->second;

	#pragma region SetAnimation


	#pragma endregion
}

#endif
