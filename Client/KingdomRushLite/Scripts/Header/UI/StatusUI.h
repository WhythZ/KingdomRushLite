#ifndef _STATUS_UI_H_
#define _STATUS_UI_H_

#include <SDL.h>
//本类中关于UI的渲染是直接以像素点为单位进行定位的，所以使用SDL_Point这个整型坐标点二维向量，而无需涉及浮点数运算
//#include "../Infra/Vector2.h"

//状态栏UI，第一排是家头像+等于生命值的血量图标（实时）+金币图标+金币数值（实时），第二排是玩家头像+玩家技能冷却条（实时）
class StatusUI
{
private:
	#pragma region Setting
	#pragma region Size
	const SDL_Point healthHeartIconSize = { 32,32 };               //血量爱心图标尺寸
	const SDL_Point coinIconSize = { 32,32 };                      //金币图标尺寸

	SDL_Point coinNumTextSize = { 0, 0 };                          //暂存金币数文本渲染的尺寸
	SDL_Point waveNumTextSize = { 0, 0 };                          //暂存波次数文本渲染的尺寸

	const SDL_Point playerIconSize = { 65,65 };                    //玩家头像图标尺寸

	const SDL_Point mpBarSize = { 240, 24 };                       //法力值条的总体尺寸
	const int mpBarBorderThickness = 3;                            //法力值条边框的厚度
	#pragma endregion

	#pragma region Distance
	const int healthHeartIconBetweenDistance = 3;                  //每个血量图标的左右间距

	const int iconTextBetweenDistance = 5;                         //图标与文本的左右间距

	const int rowBetweenDistance = 10;                             //两行渲染元素的上下间距

	const int iconBarBetweenDistance = 5;                          //图标与数值条的左右间距
	#pragma endregion

	#pragma region Color
	const SDL_Color coinNumTextColor = { 255,255,255,255 };        //金币数量文本的固定颜色
	const SDL_Color waveNumTextColor = { 255,0,0,255 };            //波次数量文本的固定颜色

	const SDL_Color mpBarBackgroundColor = { 50,40,50,255 };       //法力值条内容背景颜色
	const SDL_Color mpBarContentColor = { 145,120,170,255 };       //法力值条内容颜色
	#pragma endregion
	#pragma endregion

	SDL_Texture* coinNumTextTexture = nullptr;                     //暂存金币数量文本转化而来的渲染纹理
	SDL_Texture* waveNumTextTexture = nullptr;                     //暂存波次数量文本转化而来的渲染纹理
	
	double skill00MpBarRatio = 1;                                  //玩家操控的角色法力值条的实时占比
	double skill01MpBarRatio = 1;

public:
	StatusUI() = default;
	~StatusUI() = default;

	void OnUpdate(SDL_Renderer*);                                  //更新文本的纹理渲染数据，故需传入渲染器
	void OnRender(SDL_Renderer*);
};

#endif
