#ifndef _RESOURCE_MANAGER_HPP_
#define _RESOURCE_MANAGER_HPP_

#include <unordered_map>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Manager.hpp"

//图像资源、音频资源、字体资源
enum SpriteResID
{
	Player,                      //玩家控制角色的帧动画
	VFX_Flash_Up,                //玩家技能闪电，方向朝上
	VFX_Flash_Down,              //玩家技能闪电，方向朝下
	VFX_Flash_Left,              //玩家技能闪电，方向朝左
	VFX_Flash_Right,             //玩家技能闪电，方向朝右
	VFX_Impact_Up,               //
	VFX_Impact_Down,             //
	VFX_Impact_Left,             //
	VFX_Impact_Right,            //
	VFX_Explode,                 //

	Tile_TileSet,                //瓦片素材图集
	Tile_Home,                   //家瓦片
	Tile_Spawner,                //怪物生成点瓦片
	Tile_DirectionArrow,         //行进路径方向

	Tower_Archer,                //弓箭手防御塔
	Tower_Axeman,                //投斧手防御塔
	Tower_Gunner,	             //枪炮手防御塔
	Bullet_Arrow,                //弓箭投射物
	Bullet_Axe,                  //斧子投射物
	Bullet_Shell,                //炮弹投射物

	Enemy_Slime,                 //史莱姆帧动画
	Enemy_SlimeKing,             //王史莱姆帧动画
	Enemy_Skeleton,              //骷髅兵帧动画
	Enemy_Goblin,                //哥布林帧动画
	Enemy_GoblinPriest,          //牧师哥布林帧动画
	Enemy_Slime_Sketch,          //史莱姆白色剪影，用作受击特效
	Enemy_SlimeKing_Sketch,      //王史莱姆白色剪影
	Enemy_Skeleton_Sketch,       //骷髅兵白色剪影
	Enemy_Goblin_Sketch,         //哥布林白色剪影
	Enemy_GoblinPriest_Sketch,   //牧师哥布林白色剪影

	Item_Coin,                   //掉落的金币

	UI_Coin,                     //玩家金币总额
	UI_Heart,                    //玩家血量红心
	UI_Avatar_Player,            //玩家头像
	UI_Avatar_Home,              //家头像

	UI_SelectCursor,             //选中对象的视效
	UI_Place_Idle,               //放置防御塔的轮盘UI
	UI_Place_HoveredTop,         //指针悬浮在放置轮盘上部时的反馈视效
	UI_Place_HoveredLeft,        //指针悬浮在放置轮盘左部时的反馈视效
	UI_Place_HoveredRight,       //指针悬浮在放置轮盘右部时的反馈视效
	UI_Upgrade_Idle,             //升级防御塔的轮盘UI
	UI_Upgrade_HoveredTop,       //指针悬浮在升级轮盘上部时的反馈视效
	UI_Upgrade_HoveredLeft,      //指针悬浮在升级轮盘左部时的反馈视效
	UI_Upgrade_HoveredRight,     //指针悬浮在升级轮盘右部时的反馈视效

	UI_Text_Win,                 //胜利文字显示
	UI_Text_Loss,                //失败文字显示
	UI_GameOver                  //失败界面
};

enum AudioResID
{
	BGM_Main,                    //主背景音乐

	SFX_PlayerSkill_Flash,       //玩家技能音效
	SFX_PlayerSkill_Impact,      //玩家技能音效

	SFX_Pick_Coin,               //硬币捡拾音效
	SFX_HomeHurt,                //家被攻击音效

	SFX_Tower_Place,             //防御塔放置音效
	SFX_Tower_Upgrade,           //防御塔升级音效

	SFX_Arrow_Shoot_1,           //箭矢投射音效1
	SFX_Arrow_Shoot_2,           //箭矢投射音效2
	SFX_Arrow_Hit_1,             //箭矢命中音效1
	SFX_Arrow_Hit_2,             //箭矢命中音效2
	SFX_Arrow_Hit_3,             //箭矢命中音效3

	SFX_Axe_Shoot,               //斧头投射音效
	SFX_Axe_Hit_1,               //斧头命中音效1
	SFX_Axe_Hit_2,               //斧头命中音效2
	SFX_Axe_Hit_3,               //斧头命中音效3

	SFX_Shell_Shoot,             //炸弹投射音效
	SFX_Shell_Hit,               //炸弹命中音效

	SFX_Win,                     //游戏胜利音效
	SFX_Loss,                    //游戏失败音效
};

enum FontResID
{
	Pixel_CN,                   //游戏主字体（含中文）
};

//图像池类、音乐池类、声音池类、字体池类
typedef std::unordered_map<SpriteResID, SDL_Texture*> TexturePool;
typedef std::unordered_map<AudioResID, Mix_Music*> MusicPool;
typedef std::unordered_map<AudioResID, Mix_Chunk*> SoundPool;
typedef std::unordered_map<FontResID, TTF_Font*> FontPool;

//游戏资产管理器
class ResourceManager :public Manager<ResourceManager>
{
	friend class Manager<ResourceManager>;

private:
	TexturePool texturePool;              //图像池
	MusicPool musicPool;                  //音乐池
	SoundPool soundPool;                  //音效池
	FontPool fontPool;                    //字体池

public:
	bool LoadResource(SDL_Renderer*);     //加载所有资源

	const TexturePool& GetTexturePool();  //获取只读图像池
	const MusicPool& GetMusicPool();      //获取只读音频池
	const SoundPool& GetSoundPool();      //获取只读音效池
	const FontPool& GetFontPool();        //获取只读图像池

private:
	ResourceManager() = default;
	~ResourceManager() = default;

	bool LoadTextureRes(SDL_Renderer*);   //从文件中加载图像资产
	bool LoadMusicRes();                  //从文件中加载音乐资产
	bool LoadSoundRes();                  //从文件中加载音效资产
	bool LoadFontRes();                   //从文件中加载字体资产
};

bool ResourceManager::LoadResource(SDL_Renderer* _renderer)
{
	if (!LoadTextureRes(_renderer)) return false;
	if (!LoadMusicRes()) return false;
	if (!LoadSoundRes()) return false;
	if (!LoadFontRes()) return false;

	return true;
}

const TexturePool& ResourceManager::GetTexturePool()
{
	return texturePool;
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

bool ResourceManager::LoadTextureRes(SDL_Renderer* _renderer)
{
	//因为此处我们无需用到SDL_Surface*对象，所以可以直接使用IMG_LoadTexture()方法省略将两个步骤合一
	//SDL_Surface* _imgSurface = IMG_Load("Assets/xx.jpg");                             //从外存加载到内存（内存结构体）
	//SDL_Texture* _imgTexture = SDL_CreateTextureFromSurface(_renderer, _imgSurface);  //从内存加载到显存（GPU纹理数据）

	//将图片资源加载存储在图像池（unordered_map类型的容器）
	texturePool[SpriteResID::Player] = IMG_LoadTexture(_renderer, "Assets/Sprite/Player/player.png");
	texturePool[SpriteResID::VFX_Flash_Up] = IMG_LoadTexture(_renderer, "Assets/Sprite/VFX/flash_up.png");
	texturePool[SpriteResID::VFX_Flash_Down] = IMG_LoadTexture(_renderer, "Assets/Sprite/VFX/flash_down.png");
	texturePool[SpriteResID::VFX_Flash_Left] = IMG_LoadTexture(_renderer, "Assets/Sprite/VFX/flash_left.png");
	texturePool[SpriteResID::VFX_Flash_Right] = IMG_LoadTexture(_renderer, "Assets/Sprite/VFX/flash_right.png");
	texturePool[SpriteResID::VFX_Impact_Up] = IMG_LoadTexture(_renderer, "Assets/Sprite/VFX/impact_up.png");
	texturePool[SpriteResID::VFX_Impact_Down] = IMG_LoadTexture(_renderer, "Assets/Sprite/VFX/impact_down.png");
	texturePool[SpriteResID::VFX_Impact_Left] = IMG_LoadTexture(_renderer, "Assets/Sprite/VFX/impact_left.png");
	texturePool[SpriteResID::VFX_Impact_Right] = IMG_LoadTexture(_renderer, "Assets/Sprite/VFX/impact_right.png");
	texturePool[SpriteResID::VFX_Explode] = IMG_LoadTexture(_renderer, "Assets/Sprite/VFX/explode.png");

	texturePool[SpriteResID::Tile_TileSet] = IMG_LoadTexture(_renderer, "Assets/Sprite/Tile/tileset.png");
	texturePool[SpriteResID::Tile_Home] = IMG_LoadTexture(_renderer, "Assets/Sprite/Tile/home.png");
	texturePool[SpriteResID::Tile_Spawner] = IMG_LoadTexture(_renderer, "Assets/Sprite/Tile/spawner.png");
	texturePool[SpriteResID::Tile_DirectionArrow] = IMG_LoadTexture(_renderer, "Assets/Sprite/Tile/direction_arrow.png");

	texturePool[SpriteResID::Tower_Archer] = IMG_LoadTexture(_renderer, "Assets/Sprite/Tower/tower_archer.png");
	texturePool[SpriteResID::Tower_Axeman] = IMG_LoadTexture(_renderer, "Assets/Sprite/Tower/tower_axeman.png");
	texturePool[SpriteResID::Tower_Gunner] = IMG_LoadTexture(_renderer, "Assets/Sprite/Tower/tower_gunner.png");
	texturePool[SpriteResID::Bullet_Arrow] = IMG_LoadTexture(_renderer, "Assets/Sprite/Bullet/arrow.png");
	texturePool[SpriteResID::Bullet_Axe] = IMG_LoadTexture(_renderer, "Assets/Sprite/Bullet/axe.png");
	texturePool[SpriteResID::Bullet_Shell] = IMG_LoadTexture(_renderer, "Assets/Sprite/Bullet/shell.png");

	texturePool[SpriteResID::Enemy_Slime] = IMG_LoadTexture(_renderer, "Assets/Sprite/Enemy/slime.png");
	texturePool[SpriteResID::Enemy_Slime_Sketch] = IMG_LoadTexture(_renderer, "Assets/Sprite/Enemy/slime_sketch.png");
	texturePool[SpriteResID::Enemy_SlimeKing] = IMG_LoadTexture(_renderer, "Assets/Sprite/Enemy/king_slime.png");
	texturePool[SpriteResID::Enemy_SlimeKing_Sketch] = IMG_LoadTexture(_renderer, "Assets/Sprite/Enemy/king_slime_sketch.png");
	texturePool[SpriteResID::Enemy_Skeleton] = IMG_LoadTexture(_renderer, "Assets/Sprite/Enemy/skeleton.png");
	texturePool[SpriteResID::Enemy_Skeleton_Sketch] = IMG_LoadTexture(_renderer, "Assets/Sprite/Enemy/skeleton_sketch.png");
	texturePool[SpriteResID::Enemy_Goblin] = IMG_LoadTexture(_renderer, "Assets/Sprite/Enemy/goblin.png");
	texturePool[SpriteResID::Enemy_Goblin_Sketch] = IMG_LoadTexture(_renderer, "Assets/Sprite/Enemy/goblin_sketch.png");
	texturePool[SpriteResID::Enemy_GoblinPriest] = IMG_LoadTexture(_renderer, "Assets/Sprite/Enemy/priest_goblin.png");
	texturePool[SpriteResID::Enemy_GoblinPriest_Sketch] = IMG_LoadTexture(_renderer, "Assets/Sprite/Enemy/priest_goblin_sketch.png");

	texturePool[SpriteResID::Item_Coin] = IMG_LoadTexture(_renderer, "Assets/Sprite/Item/coin.png");

	texturePool[SpriteResID::UI_Coin] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/coin.png");
	texturePool[SpriteResID::UI_Heart] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/heart.png");
	texturePool[SpriteResID::UI_Avatar_Player] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/player_avatar.png");
	texturePool[SpriteResID::UI_Avatar_Home] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/home_avatar.png");

	texturePool[SpriteResID::UI_SelectCursor] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/select_cursor.png");
	texturePool[SpriteResID::UI_Place_Idle] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/place_idle.png");
	texturePool[SpriteResID::UI_Place_HoveredTop] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/place_hovered_top.png");
	texturePool[SpriteResID::UI_Place_HoveredLeft] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/place_hovered_left.png");
	texturePool[SpriteResID::UI_Place_HoveredRight] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/place_hovered_right.png");
	texturePool[SpriteResID::UI_Upgrade_Idle] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/upgrade_idle.png");
	texturePool[SpriteResID::UI_Upgrade_HoveredTop] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/upgrade_hovered_top.png");
	texturePool[SpriteResID::UI_Upgrade_HoveredLeft] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/upgrade_hovered_left.png");
	texturePool[SpriteResID::UI_Upgrade_HoveredRight] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/upgrade_hovered_right.png");

	texturePool[SpriteResID::UI_Text_Win] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/win_text.png");
	texturePool[SpriteResID::UI_Text_Loss] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/loss_text.png");
	texturePool[SpriteResID::UI_GameOver] = IMG_LoadTexture(_renderer, "Assets/Sprite/UI/game_over_bar.png");

	//检查unordered_map容器中所有键值对的值是否有效；其中auto是自动类型推导，成员second访问的是键值对的值
	for (const auto& _pair : texturePool)
		if (!_pair.second) return false;
	//检查无误就返回加载成功
	return true;
}

bool ResourceManager::LoadMusicRes()
{
	//从文件中加载音乐
	musicPool[AudioResID::BGM_Main] = Mix_LoadMUS("Assets/Audio/BGM/main_bgm.mp3");

	//检查unordered_map容器中所有键值对的值是否有效；其中auto是自动类型推导，成员second访问的是键值对的值
	for (const auto& _pair : musicPool)
		if (!_pair.second) return false;
	//检查无误就返回加载成功
	return true;
}

bool ResourceManager::LoadSoundRes()
{
	//从文件中加载音效
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

	//检查unordered_map容器中所有键值对的值是否有效；其中auto是自动类型推导，成员second访问的是键值对的值
	for (const auto& _pair : soundPool)
		if (!_pair.second) return false;
	//检查无误就返回加载成功
	return true;
}

bool ResourceManager::LoadFontRes()
{
	//从文件中加载字体
	fontPool[FontResID::Pixel_CN] = TTF_OpenFont("Assets/Font/ipix.ttf", 25);

	//检查unordered_map容器中所有键值对的值是否有效；其中auto是自动类型推导，成员second访问的是键值对的值
	for (const auto& _pair : fontPool)
		if (!_pair.second) return false;
	//检查无误就返回加载成功
	return true;
}

#endif
