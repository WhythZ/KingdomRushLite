#ifndef _RESOURCE_MANAGER_HPP_
#define _RESOURCE_MANAGER_HPP_

#include <unordered_map>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Manager.hpp"

//ͼ����Դ����Ƶ��Դ��������Դ
enum SpriteResID
{
	Player,                      //��ҿ��ƽ�ɫ��֡����
	VFX_Flash_Up,                //��Ҽ������磬������
	VFX_Flash_Down,              //��Ҽ������磬������
	VFX_Flash_Left,              //��Ҽ������磬������
	VFX_Flash_Right,             //��Ҽ������磬������
	VFX_Impact_Up,               //
	VFX_Impact_Down,             //
	VFX_Impact_Left,             //
	VFX_Impact_Right,            //
	VFX_Explode,                 //

	Tile_TileSet,                //��Ƭ�ز�ͼ��
	Tile_Home,                   //����Ƭ
	Tile_Spawner,                //�������ɵ���Ƭ
	Tile_DirectionArrow,         //�н�·������

	Tower_Archer,                //�����ַ�����
	Tower_Axeman,                //Ͷ���ַ�����
	Tower_Gunner,	             //ǹ���ַ�����
	Bullet_Arrow,                //����Ͷ����
	Bullet_Axe,                  //����Ͷ����
	Bullet_Shell,                //�ڵ�Ͷ����

	Enemy_Slime,                 //ʷ��ķ֡����
	Enemy_KingSlime,             //��ʷ��ķ֡����
	Enemy_Skeleton,              //���ñ�֡����
	Enemy_Goblin,                //�粼��֡����
	Enemy_PriestGoblin,          //��ʦ�粼��֡����
	Enemy_Slime_Sketch,          //ʷ��ķ��ɫ��Ӱ�������ܻ���Ч
	Enemy_KingSlime_Sketch,      //��ʷ��ķ��ɫ��Ӱ
	Enemy_Skeleton_Sketch,       //���ñ���ɫ��Ӱ
	Enemy_Goblin_Sketch,         //�粼�ְ�ɫ��Ӱ
	Enemy_PriestGoblin_Sketch,   //��ʦ�粼�ְ�ɫ��Ӱ

	Item_Coin,                   //����Ľ��

	UI_Coin,                     //��ҽ���ܶ�
	UI_Heart,                    //���Ѫ������
	UI_Avatar_Player,            //���ͷ��
	UI_Avatar_Home,              //��ͷ��

	UI_SelectCursor,             //ѡ�ж������Ч
	UI_Place_Idle,               //���÷�����������UI
	UI_Place_HoveredTop,         //ָ�������ڷ��������ϲ�ʱ�ķ�����Ч
	UI_Place_HoveredLeft,        //ָ�������ڷ���������ʱ�ķ�����Ч
	UI_Place_HoveredRight,       //ָ�������ڷ��������Ҳ�ʱ�ķ�����Ч
	UI_Upgrade_Idle,             //����������������UI
	UI_Upgrade_HoveredTop,       //ָ�����������������ϲ�ʱ�ķ�����Ч
	UI_Upgrade_HoveredLeft,      //ָ������������������ʱ�ķ�����Ч
	UI_Upgrade_HoveredRight,     //ָ�����������������Ҳ�ʱ�ķ�����Ч

	UI_Text_Win,                 //ʤ��������ʾ
	UI_Text_Loss,                //ʧ��������ʾ
	UI_GameOver                  //ʧ�ܽ���
};

enum AudioResID
{
	BGM_Main,                    //����������

	SFX_PlayerSkill_Flash,       //��Ҽ�����Ч
	SFX_PlayerSkill_Impact,      //��Ҽ�����Ч

	SFX_Pick_Coin,               //Ӳ�Ҽ�ʰ��Ч
	SFX_HomeHurt,                //�ұ�������Ч

	SFX_Tower_Place,             //������������Ч
	SFX_Tower_Upgrade,           //������������Ч

	SFX_Arrow_Shoot_1,           //��ʸͶ����Ч1
	SFX_Arrow_Shoot_2,           //��ʸͶ����Ч2
	SFX_Arrow_Hit_1,             //��ʸ������Ч1
	SFX_Arrow_Hit_2,             //��ʸ������Ч2
	SFX_Arrow_Hit_3,             //��ʸ������Ч3

	SFX_Axe_Shoot,               //��ͷͶ����Ч
	SFX_Axe_Hit_1,               //��ͷ������Ч1
	SFX_Axe_Hit_2,               //��ͷ������Ч2
	SFX_Axe_Hit_3,               //��ͷ������Ч3

	SFX_Shell_Shoot,             //ը��Ͷ����Ч
	SFX_Shell_Hit,               //ը��������Ч

	SFX_Win,                     //��Ϸʤ����Ч
	SFX_Loss,                    //��Ϸʧ����Ч
};

enum FontResID
{
	Pixel_CN,                   //��Ϸ�����壨�����ģ�
};

//ͼ����ࡢ���ֳ��ࡢ�������ࡢ�������
typedef std::unordered_map<SpriteResID, SDL_Texture*> SpritePool;
typedef std::unordered_map<AudioResID, Mix_Music*> MusicPool;
typedef std::unordered_map<AudioResID, Mix_Chunk*> SoundPool;
typedef std::unordered_map<FontResID, TTF_Font*> FontPool;

//��Ϸ�ʲ�������
class ResourceManager :public Manager<ResourceManager>
{
	friend class Manager<ResourceManager>;

private:
	SpritePool spritePool;              //ͼ���
	MusicPool musicPool;                //���ֳ�
	SoundPool soundPool;                //��Ч��
	FontPool fontPool;                  //�����

public:
	bool LoadResource(SDL_Renderer*);   //����������Դ

	const SpritePool& GetSpritePool();  //��ȡֻ��ͼ���
	const MusicPool& GetMusicPool();    //��ȡֻ����Ƶ��
	const SoundPool& GetSoundPool();    //��ȡֻ����Ч��
	const FontPool& GetFontPool();      //��ȡֻ��ͼ���

private:
	ResourceManager() = default;
	~ResourceManager() = default;

	bool LoadSpriteRes(SDL_Renderer*);  //���ļ��м���ͼ���ʲ�
	bool LoadMusicRes();                //���ļ��м��������ʲ�
	bool LoadSoundRes();                //���ļ��м�����Ч�ʲ�
	bool LoadFontRes();                 //���ļ��м��������ʲ�
};

bool ResourceManager::LoadResource(SDL_Renderer* _renderer)
{
	if (!LoadSpriteRes(_renderer)) return false;
	if (!LoadMusicRes()) return false;
	if (!LoadSoundRes()) return false;
	if (!LoadFontRes()) return false;

	return true;
}

const SpritePool& ResourceManager::GetSpritePool()
{
	return spritePool;
}

const MusicPool& ResourceManager::GetMusicPool()
{
	return musicPool;
}

const SoundPool& ResourceManager::GetSoundPool()
{
	return soundPool;
}

const FontPool& ResourceManager::GetFontPool()
{
	return fontPool;
}

bool ResourceManager::LoadSpriteRes(SDL_Renderer* _renderer)
{
	//��Ϊ�˴����������õ�SDL_Surface*�������Կ���ֱ��ʹ��IMG_LoadTexture()����ʡ�Խ����������һ
	//SDL_Surface* _imgSurface = IMG_Load("Assets/xx.jpg");                             //�������ص��ڴ棨�ڴ�ṹ�壩
	//SDL_Texture* _imgTexture = SDL_CreateTextureFromSurface(_renderer, _imgSurface);  //���ڴ���ص��Դ棨GPU�������ݣ�

	//��ͼƬ��Դ���ش洢��ͼ��أ�unordered_map���͵�������
	spritePool[SpriteResID::Player] = IMG_LoadTexture(_renderer, "Assets/Sprite/Player/player.png");
	spritePool[SpriteResID::VFX_Flash_Up] = IMG_LoadTexture(_renderer, "Assets/Sprite/VFX/flash_up.png");
	spritePool[SpriteResID::VFX_Flash_Down] = IMG_LoadTexture(_renderer, "Assets/Sprite/VFX/flash_down.png");
	spritePool[SpriteResID::VFX_Flash_Left] = IMG_LoadTexture(_renderer, "Assets/Sprite/VFX/flash_left.png");
	spritePool[SpriteResID::VFX_Flash_Right] = IMG_LoadTexture(_renderer, "Assets/Sprite/VFX/flash_right.png");
	spritePool[SpriteResID::VFX_Impact_Up] = IMG_LoadTexture(_renderer, "Assets/Sprite/VFX/impact_up.png");
	spritePool[SpriteResID::VFX_Impact_Down] = IMG_LoadTexture(_renderer, "Assets/Sprite/VFX/impact_down.png");
	spritePool[SpriteResID::VFX_Impact_Left] = IMG_LoadTexture(_renderer, "Assets/Sprite/VFX/impact_left.png");
	spritePool[SpriteResID::VFX_Impact_Right] = IMG_LoadTexture(_renderer, "Assets/Sprite/VFX/impact_right.png");
	spritePool[SpriteResID::VFX_Explode] = IMG_LoadTexture(_renderer, "Assets/Sprite/VFX/explode.png");

	spritePool[SpriteResID::Tile_TileSet] = IMG_LoadTexture(_renderer, "Assets/Sprite/Tile/tileset.png");
	spritePool[SpriteResID::Tile_Home] = IMG_LoadTexture(_renderer, "Assets/Sprite/Tile/home.png");
	spritePool[SpriteResID::Tile_Spawner] = IMG_LoadTexture(_renderer, "Assets/Sprite/Tile/spawner.png");
	spritePool[SpriteResID::Tile_DirectionArrow] = IMG_LoadTexture(_renderer, "Assets/Sprite/Tile/direction_arrow.png");

	spritePool[SpriteResID::Tower_Archer] = IMG_LoadTexture(_renderer, "Assets/Sprite/Tower/tower_archer.png");
	spritePool[SpriteResID::Tower_Axeman] = IMG_LoadTexture(_renderer, "Assets/Sprite/Tower/tower_axeman.png");
	spritePool[SpriteResID::Tower_Gunner] = IMG_LoadTexture(_renderer, "Assets/Sprite/Tower/tower_gunner.png");
	spritePool[SpriteResID::Bullet_Arrow] = IMG_LoadTexture(_renderer, "Assets/Sprite/Bullet/arrow.png");
	spritePool[SpriteResID::Bullet_Axe] = IMG_LoadTexture(_renderer, "Assets/Sprite/Bullet/axe.png");
	spritePool[SpriteResID::Bullet_Shell] = IMG_LoadTexture(_renderer, "Assets/Sprite/Bullet/shell.png");

	spritePool[SpriteResID::Enemy_Slime] = IMG_LoadTexture(_renderer, "Assets/Sprite/Enemy/slime.png");
	spritePool[SpriteResID::Enemy_Slime_Sketch] = IMG_LoadTexture(_renderer, "Assets/Sprite/Enemy/slime_sketch.png");
	spritePool[SpriteResID::Enemy_KingSlime] = IMG_LoadTexture(_renderer, "Assets/Sprite/Enemy/king_slime.png");
	spritePool[SpriteResID::Enemy_KingSlime_Sketch] = IMG_LoadTexture(_renderer, "Assets/Sprite/Enemy/king_slime_sketch.png");
	spritePool[SpriteResID::Enemy_Skeleton] = IMG_LoadTexture(_renderer, "Assets/Sprite/Enemy/skeleton.png");
	spritePool[SpriteResID::Enemy_Skeleton_Sketch] = IMG_LoadTexture(_renderer, "Assets/Sprite/Enemy/skeleton_sketch.png");
	spritePool[SpriteResID::Enemy_Goblin] = IMG_LoadTexture(_renderer, "Assets/Sprite/Enemy/goblin.png");
	spritePool[SpriteResID::Enemy_Goblin_Sketch] = IMG_LoadTexture(_renderer, "Assets/Sprite/Enemy/goblin_sketch.png");
	spritePool[SpriteResID::Enemy_PriestGoblin] = IMG_LoadTexture(_renderer, "Assets/Sprite/Enemy/priest_goblin.png");
	spritePool[SpriteResID::Enemy_PriestGoblin_Sketch] = IMG_LoadTexture(_renderer, "Assets/Sprite/Enemy/priest_goblin_sketch.png");

	spritePool[SpriteResID::Item_Coin] = IMG_LoadTexture(_renderer, "Assets/Sprite/Item/coin.png");

	spritePool[SpriteResID::UI_Coin] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/coin.png");
	spritePool[SpriteResID::UI_Heart] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/heart.png");
	spritePool[SpriteResID::UI_Avatar_Player] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/player_avatar.png");
	spritePool[SpriteResID::UI_Avatar_Home] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/home_avatar.png");

	spritePool[SpriteResID::UI_SelectCursor] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/select_cursor.png");
	spritePool[SpriteResID::UI_Place_Idle] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/place_idle.png");
	spritePool[SpriteResID::UI_Place_HoveredTop] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/place_hovered_top.png");
	spritePool[SpriteResID::UI_Place_HoveredLeft] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/place_hovered_left.png");
	spritePool[SpriteResID::UI_Place_HoveredRight] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/place_hovered_right.png");
	spritePool[SpriteResID::UI_Upgrade_Idle] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/upgrade_idle.png");
	spritePool[SpriteResID::UI_Upgrade_HoveredTop] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/upgrade_hovered_top.png");
	spritePool[SpriteResID::UI_Upgrade_HoveredLeft] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/upgrade_hovered_left.png");
	spritePool[SpriteResID::UI_Upgrade_HoveredRight] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/upgrade_hovered_right.png");

	spritePool[SpriteResID::UI_Text_Win] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/win_text.png");
	spritePool[SpriteResID::UI_Text_Loss] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/loss_text.png");
	spritePool[SpriteResID::UI_GameOver] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/game_over_bar.png");

	//���unordered_map���������м�ֵ�Ե�ֵ�Ƿ���Ч������auto���Զ������Ƶ�����Աsecond���ʵ��Ǽ�ֵ�Ե�ֵ
	for (const auto& _pair : spritePool)
		if (!_pair.second) return false;
	//�������ͷ��ؼ��سɹ�
	return true;
}

bool ResourceManager::LoadMusicRes()
{
	//���ļ��м�������
	musicPool[AudioResID::BGM_Main] = Mix_LoadMUS("Assets/Audio/BGM/main_bgm.mp3");

	//���unordered_map���������м�ֵ�Ե�ֵ�Ƿ���Ч������auto���Զ������Ƶ�����Աsecond���ʵ��Ǽ�ֵ�Ե�ֵ
	for (const auto& _pair : musicPool)
		if (!_pair.second) return false;
	//�������ͷ��ؼ��سɹ�
	return true;
}

bool ResourceManager::LoadSoundRes()
{
	//���ļ��м�����Ч
	soundPool[AudioResID::SFX_PlayerSkill_Flash] = Mix_LoadWAV("Assets/Audio/SFX/flash.wav");
	soundPool[AudioResID::SFX_PlayerSkill_Impact] = Mix_LoadWAV("Assets/Audio/SFX/impact.wav");

	soundPool[AudioResID::SFX_Pick_Coin] = Mix_LoadWAV("Assets/Audio/SFX/coin.mp3");
	soundPool[AudioResID::SFX_HomeHurt] = Mix_LoadWAV("Assets/Audio/SFX/home_hurt.wav");

	soundPool[AudioResID::SFX_Tower_Place] = Mix_LoadWAV("Assets/Audio/SFX/place_tower.mp3");
	soundPool[AudioResID::SFX_Tower_Upgrade] = Mix_LoadWAV("Assets/Audio/SFX/tower_level_up.mp3");

	soundPool[AudioResID::SFX_Arrow_Shoot_1] = Mix_LoadWAV("Assets/Audio/SFX/arrow_fire_1.mp3");
	soundPool[AudioResID::SFX_Arrow_Shoot_2] = Mix_LoadWAV("Assets/Audio/SFX/arrow_fire_2.mp3");
	soundPool[AudioResID::SFX_Arrow_Hit_1] = Mix_LoadWAV("Assets/Audio/SFX/arrow_hit_1.mp3");
	soundPool[AudioResID::SFX_Arrow_Hit_2] = Mix_LoadWAV("Assets/Audio/SFX/arrow_hit_2.mp3");
	soundPool[AudioResID::SFX_Arrow_Hit_3] = Mix_LoadWAV("Assets/Audio/SFX/arrow_hit_3.mp3");

	soundPool[AudioResID::SFX_Axe_Shoot] = Mix_LoadWAV("Assets/Audio/SFX/axe_fire.wav");
	soundPool[AudioResID::SFX_Axe_Hit_1] = Mix_LoadWAV("Assets/Audio/SFX/axe_hit_1.mp3");
	soundPool[AudioResID::SFX_Axe_Hit_2] = Mix_LoadWAV("Assets/Audio/SFX/axe_hit_2.mp3");
	soundPool[AudioResID::SFX_Axe_Hit_3] = Mix_LoadWAV("Assets/Audio/SFX/axe_hit_3.mp3");

	soundPool[AudioResID::SFX_Shell_Shoot] = Mix_LoadWAV("Assets/Audio/SFX/shell_fire.wav");
	soundPool[AudioResID::SFX_Shell_Hit] = Mix_LoadWAV("Assets/Audio/SFX/shell_hit.mp3");

	soundPool[AudioResID::SFX_Win] = Mix_LoadWAV("Assets/Audio/SFX/win.wav");
	soundPool[AudioResID::SFX_Loss] = Mix_LoadWAV("Assets/Audio/SFX/loss.mp3");

	//���unordered_map���������м�ֵ�Ե�ֵ�Ƿ���Ч������auto���Զ������Ƶ�����Աsecond���ʵ��Ǽ�ֵ�Ե�ֵ
	for (const auto& _pair : soundPool)
		if (!_pair.second) return false;
	//�������ͷ��ؼ��سɹ�
	return true;
}

bool ResourceManager::LoadFontRes()
{
	//���ļ��м�������
	fontPool[FontResID::Pixel_CN] = TTF_OpenFont("Assets/Font/ipix.ttf", 25);

	//���unordered_map���������м�ֵ�Ե�ֵ�Ƿ���Ч������auto���Զ������Ƶ�����Աsecond���ʵ��Ǽ�ֵ�Ե�ֵ
	for (const auto& _pair : fontPool)
		if (!_pair.second) return false;
	//�������ͷ��ؼ��سɹ�
	return true;
}

#endif
