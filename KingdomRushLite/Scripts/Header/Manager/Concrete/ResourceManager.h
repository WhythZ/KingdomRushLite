#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_

#include <unordered_map>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "../Manager.hpp"

//图像资源、音频资源、字体资源
enum class TextureResID
{
	Player_Dragon,                  //玩家控制的龙角色的帧动画
	
	VFX_Player_Dragon_Flash_Up,     //玩家龙技能闪电，方向朝上
	VFX_Player_Dragon_Flash_Down,   //玩家龙技能闪电，方向朝下
	VFX_Player_Dragon_Flash_Left,   //玩家龙技能闪电，方向朝左
	VFX_Player_Dragon_Flash_Right,  //玩家龙技能闪电，方向朝右
	VFX_Player_Dragon_Impact_Up,    //玩家龙技能震慑，方向朝右
	VFX_Player_Dragon_Impact_Down,  //玩家龙技能震慑，方向朝右
	VFX_Player_Dragon_Impact_Left,  //玩家龙技能震慑，方向朝右
	VFX_Player_Dragon_Impact_Right, //玩家龙技能震慑，方向朝右

	Tile_TileSet,                   //瓦片素材图集
	Tile_Home,                      //家瓦片
	Tile_Spawner,                   //怪物生成点瓦片
	Tile_DirectionArrow,            //行进路径方向

	Tower_Archer,                   //弓箭手防御塔
	Tower_Axeman,                   //投斧手防御塔
	Tower_Gunner,	                //枪炮手防御塔
	
	Bullet_Arrow,                   //弓箭投射物
	Bullet_Axe,                     //斧子投射物
	Bullet_Shell,                   //炮弹投射物
	Bullet_Shell_Explode,           //炮弹投射物爆炸

	Enemy_Slime,                    //史莱姆帧动画
	Enemy_Slime_Sketch,             //史莱姆白色剪影，用作受击特效
	Enemy_SlimeKing,                //王史莱姆帧动画
	Enemy_SlimeKing_Sketch,         //王史莱姆白色剪影
	Enemy_Skeleton,                 //骷髅兵帧动画
	Enemy_Skeleton_Sketch,          //骷髅兵白色剪影
	Enemy_Goblin,                   //哥布林帧动画
	Enemy_Goblin_Sketch,            //哥布林白色剪影
	Enemy_GoblinPriest,             //牧师哥布林帧动画
	Enemy_GoblinPriest_Sketch,      //牧师哥布林白色剪影

	Item_Coin,                      //掉落的金币

	UI_Icon_Coin,                   //玩家金币总额
	UI_Icon_HealthHeart,            //玩家血量红心
	UI_Icon_Player_Dragon,          //玩家龙头像图标
	UI_Icon_Home,                   //家头像图标

	UI_SelectCursor,                //选中瓦片的效果
	UI_Build_Idle,                  //放置防御塔的轮盘UI
	UI_Build_HoveredTop,            //指针悬浮在放置轮盘上部时的反馈视效
	UI_Build_HoveredLeft,           //指针悬浮在放置轮盘左部时的反馈视效
	UI_Build_HoveredRight,          //指针悬浮在放置轮盘右部时的反馈视效
	UI_Upgrade_Idle,                //升级防御塔的轮盘UI
	UI_Upgrade_HoveredTop,          //指针悬浮在升级轮盘上部时的反馈视效
	UI_Upgrade_HoveredLeft,         //指针悬浮在升级轮盘左部时的反馈视效
	UI_Upgrade_HoveredRight,        //指针悬浮在升级轮盘右部时的反馈视效
	
	UI_Text_Win,                    //胜利文字显示
	UI_Text_Loss,                   //失败文字显示
	UI_Bar_GameOver                 //游戏结算界面条
};

enum class MusicResID
{
	BGM_InLevel_0
};

enum class SoundResID
{
	Player_Dragon_Skill_Flash,      //玩家龙技能闪电音效
	Player_Dragon_Skill_Impact,     //玩家龙技能震慑音效

	Pick_Coin,                      //硬币捡拾音效
	Home_Hurt,                      //家被攻击音效

	Tower_Build,                    //防御塔放置音效
	Tower_Upgrade,                  //防御塔升级音效
	Tower_Error,                    //防御塔操作失败音效

	Bullet_Arrow_Fire_1,            //箭矢投射音效1
	Bullet_Arrow_Fire_2,            //箭矢投射音效2
	Bullet_Arrow_Hit_1,             //箭矢命中音效1
	Bullet_Arrow_Hit_2,             //箭矢命中音效2
	Bullet_Arrow_Hit_3,             //箭矢命中音效3

	Bullet_Axe_Fire,                //斧头投射音效
	Bullet_Axe_Hit_1,               //斧头命中音效1
	Bullet_Axe_Hit_2,               //斧头命中音效2
	Bullet_Axe_Hit_3,               //斧头命中音效3

	Bullet_Shell_Fire,              //炸弹投射音效
	Bullet_Shell_Hit,               //炸弹命中音效

	Game_Win,                       //游戏胜利音效
	Game_Loss,                      //游戏失败音效
};

enum class FontResID
{
	Ipix,
	VonwaonBitmap12,
	VonwaonBitmap16
};

//图像池类、音乐池类、声音池类、字体池类
typedef std::unordered_map<TextureResID, SDL_Texture*> TexturePool;
typedef std::unordered_map<MusicResID, Mix_Music*> MusicPool;
typedef std::unordered_map<SoundResID, Mix_Chunk*> SoundPool;
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

#endif
