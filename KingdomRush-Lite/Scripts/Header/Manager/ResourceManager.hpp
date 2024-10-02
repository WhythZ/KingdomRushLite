#ifndef _RESOURCE_MANAGER_HPP_
#define _RESOURCE_MANAGER_HPP_

#include "Manager.hpp"

//图像资源池
enum SpriteResPool
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

	Tower_Archer,                //弓箭手防御塔
	Tower_Axeman,                //投斧手防御塔
	Tower_Gunner,	             //枪炮手防御塔
	Bullet_Arrow,                //弓箭投射物
	Bullet_Axe,                  //斧子投射物
	Bullet_Shell,                //炮弹投射物

	Enemy_Slime,                 //史莱姆帧动画
	Enemy_KingSlime,             //王史莱姆帧动画
	Enemy_Skeleton,              //骷髅兵帧动画
	Enemy_Goblin,                //哥布林帧动画
	Enemy_PriestGoblin,          //牧师哥布林帧动画
	Enemy_Slime_Sketch,          //史莱姆白色剪影，用作受击特效
	Enemy_KingSlime_Sketch,      //王史莱姆白色剪影
	Enemy_Skeleton_Sketch,       //骷髅兵白色剪影
	Enemy_Goblin_Sketch,         //哥布林白色剪影
	Enemy_PriestGoblin_Sketch,   //牧师哥布林白色剪影

	UI_CoinDrop,                 //掉落的金币
	UI_CoinOwned,                //玩家金币总额
	UI_HealthBar,                //玩家生命值

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

//音频资源池
enum AudioResPool
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

//字体资源池
enum FontResPool
{
	Pixel_CN,                   //游戏主字体（含中文）
};

//游戏资产管理器
class ResourceManager :public Manager<ResourceManager>
{
	friend class Manager<ResourceManager>;

private:


public:
};

#endif
