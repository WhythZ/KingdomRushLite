#ifndef _STATUS_UI_H_
#define _STATUS_UI_H_

#include <SDL.h>
#include "../Infra/Vector2.h"

class StatusUI
{
private:
	SDL_Point position = { 0 };                                //左上顶点的坐标位置

	#pragma region Text
	SDL_Texture* coinTextTexture = nullptr;                    //暂存 金币数量文本转化而来的渲染纹理
	int coinTextWidth = 0;                                     //暂存金币文本渲染的宽度
	int coinTextHeight = 0;                                    //暂存金币文本渲染的长度

	const SDL_Color coinTextColor = { 255,255,255,255 };       //金币数量文本的固定颜色（纯白色）
	#pragma endregion

	#pragma region Bar
	const Vector2 mpBarSize = { 200, 20 };                     //玩家操控的角色的法力值条的尺寸
	const int mpBarBorderThickness = 3;                        //法力值条边框的厚度
	const SDL_Color mpBarContentColor = { 145,120,170,255 };   //法力值条内容颜色
	const SDL_Color mpBarContentBackColor = { 50,40,50,255 };  //法力值条内容背景颜色
	#pragma endregion

	#pragma region Icon
	const Vector2 hpIconSize = { 32,32 };                      //血量数值文本旁图标的尺寸
	#pragma endregion

public:
	StatusUI() = default;
	~StatusUI() = default;
	void SetPosition(SDL_Point);

	void OnUpdate(double, SDL_Renderer*);                      //更新文本的纹理渲染数据，故需传入渲染器
	void OnRender(SDL_Renderer*);
};

#endif
