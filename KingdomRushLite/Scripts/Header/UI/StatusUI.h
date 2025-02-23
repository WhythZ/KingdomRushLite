#ifndef _STATUS_UI_H_
#define _STATUS_UI_H_

#include <SDL.h>
//本类中关于UI的渲染是直接以像素点为单位进行定位的，所以使用SDL_Point这个整型坐标点二维向量，而无需涉及浮点数运算
//#include "../Infra/Vector2.h"

//各渲染元素间的间距设置
#define STATUSUI_HOME_RIGHT_DISTANCE 15          //家图标与右侧的渲染元素（首个血量图标与金币图标）的左右间距
#define STATUSUI_HEALTH_ICON_BETWEEN_DISTANCE 3  //每个血量图标的左右间距
#define STATUSUI_COIN_ICON_NUMTEXT_DISTANCE 5    //金币图标与金币数量文本的左右间距
#define STATUSUI_HOME_PLAYER_ICON_DISTANCE 15    //家图标与玩家图标间的上下间距

//状态栏UI，第一排是家头像+等于生命值的血量图标（实时）+金币图标+金币数值（实时），第二排是玩家头像+玩家技能冷却条（实时）
class StatusUI
{
private:
	SDL_Point position = { 0, 0 };                                 //整体左上顶点的坐标位置

	#pragma region CoinNumText
	SDL_Texture* coinNumTextTexture = nullptr;                     //暂存 金币数量文本转化而来的渲染纹理
	SDL_Point coinNumTextSize = { 0, 0 };                          //暂存金币文本渲染的宽度与长度

	const SDL_Color coinTextColor = { 255,255,255,255 };           //金币数量文本的固定颜色（纯白色）
	#pragma endregion

	#pragma region PlayerMpBar
	//double mpBarRatio = 0;                                         //玩家操控的角色法力值条的实时占比

	const SDL_Point mpBarSize = { 200, 20 };                       //法力值条的总体尺寸
	const int mpBarBorderThickness = 3;                            //法力值条边框的厚度
	const SDL_Color mpBarContentForeColor = { 145,120,170,255 };   //法力值条内容颜色
	const SDL_Color mpBarContentBackColor = { 50,40,50,255 };      //法力值条内容背景颜色
	#pragma endregion

public:
	StatusUI() = default;
	~StatusUI() = default;
	void SetPosition(SDL_Point);

	void OnUpdate(SDL_Renderer*);                                  //更新文本的纹理渲染数据，故需传入渲染器
	void OnRender(SDL_Renderer*);
};

#endif
