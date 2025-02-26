#include "../../../Header/Enemy/Concrete/Slime.h"
#include "../../../Header/Manager/Concrete/ConfigManager.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"

Slime::Slime()
{
	//���ù�������
	type = EnemyType::Slime;

	//�ߴ����Ƭ��С����һ��
	size.x = TILE_SIZE;
	size.y = TILE_SIZE;

	//��ȡ��������
	static const ConfigManager::EnemyConfigPrefab& _configPrefab = ConfigManager::Instance()->slimePrefab;

	#pragma region SetAnimation
	//��ȡ����
	static SDL_Texture* _sprites = ResourceManager::Instance()->
		GetTexturePool().find(TextureResID::Enemy_Slime)->second;
	static SDL_Texture* _spritesSketch = ResourceManager::Instance()->
		GetTexturePool().find(TextureResID::Enemy_Slime_Sketch)->second;

	static const std::vector<int> _spriteIdxListDown = { 0,1,2,3,4,5 };
	static const std::vector<int> _spriteIdxListUp = { 6,7,8,9,10,11 };
	static const std::vector<int> _spriteIdxListRight = { 12,13,14,15,16,17 };
	static const std::vector<int> _spriteIdxListLeft = { 18,19,20,21,22,23 };

	//�����������ҵĶ�����ͼ
	animDown.SetLoop(true); animDown.SetAnimFrames(_sprites, 6, 4, _spriteIdxListDown);
	animUp.SetLoop(true); animUp.SetAnimFrames(_sprites, 6, 4, _spriteIdxListUp);
	animRight.SetLoop(true); animRight.SetAnimFrames(_sprites, 6, 4, _spriteIdxListRight);
	animLeft.SetLoop(true); animLeft.SetAnimFrames(_sprites, 6, 4, _spriteIdxListLeft);

	//�����������ҵ��ܻ���Ӱ��ͼ
	animDownSketch.SetLoop(true); animDownSketch.SetAnimFrames(_spritesSketch, 6, 4, _spriteIdxListDown);
	animUpSketch.SetLoop(true); animUpSketch.SetAnimFrames(_spritesSketch, 6, 4, _spriteIdxListUp);
	animRightSketch.SetLoop(true); animRightSketch.SetAnimFrames(_spritesSketch, 6, 4, _spriteIdxListRight);
	animLeftSketch.SetLoop(true); animLeftSketch.SetAnimFrames(_spritesSketch, 6, 4, _spriteIdxListLeft);
	#pragma endregion

	#pragma region SetBasicStats
	//���û�������ֵ����
	healthMaximum = _configPrefab.health;
	healthCurrent = healthMaximum;

	speedMaximum = _configPrefab.speed;
	speedCurrent = speedMaximum;

	attackDamage = _configPrefab.damage;

	coinRatio = _configPrefab.coinRatio;
	#pragma endregion

	#pragma region SetSkillStats
	//���ü��������ֵ����
	skillRecoverCooldown = _configPrefab.skillRecoverCooldown;
	skillRecoverRadius = _configPrefab.skillRecoverRadius * TILE_SIZE;
	skillRecoverIntensity = _configPrefab.skillRecoverIntensity;

	//������ȴʱ��
	skillRecoverCooldownTimer.SetWaitTime(skillRecoverCooldown);
	#pragma endregion
}
