#ifndef _TOWER_PANEL_H_
#define _TOWER_PANEL_H_

#include <SDL.h>
//以像素点为单位进行定位（坐标均为整型数），无需使用浮点数向量
//#include "../Infra/Vector2.h"

//派生建造轮盘和升级轮盘两类
class TowerPanel
{
protected:
	bool isVisible = false;                                 //决定当前轮盘UI是否可见
	SDL_Point centerPosition = { 0,0 };                     //轮盘UI中心点的像素坐标位置
	SDL_Point selectedTileIdx;                              //当前轮盘UI显示在的瓦片索引坐标

	#pragma region Textures
	SDL_Texture* idleTexture = nullptr;                     //哪个按钮都没选中时，不高亮突出任何按钮
	SDL_Texture* topTexture = nullptr;                      //选中轮盘正上方按钮，高亮突出
	SDL_Texture* leftTexture = nullptr;                     //选中轮盘左下方按钮，高亮突出
	SDL_Texture* rightTexture = nullptr;                    //选中轮盘右下方按钮，高亮突出
	#pragma endregion

private:
	#pragma region Panel
	const SDL_Point panelSize = { 144,144 };                //轮盘的整体尺寸
	#pragma endregion
	
	#pragma region Button
	const SDL_Point buttonSize = { 48,48 };                 //轮盘中各个按钮的尺寸
	const SDL_Point topButtonOffset = { 48,6 };             //正上方按钮偏移轮盘左上顶点的偏移量
	const SDL_Point leftButtonOffset = { 8,80 };            //左下方按钮偏移轮盘左上顶点的偏移量
	const SDL_Point rightButtonOffset = { 90,80 };          //右下方按钮偏移轮盘左上顶点的偏移量
	#pragma endregion

	#pragma region Text
	SDL_Point textSize = { 0,0 };
	SDL_Texture* textTexture = nullptr;

	const SDL_Color textColor = { 255,255 ,255 ,255 };
	#pragma endregion

public:
	TowerPanel() = default;
	~TowerPanel() = default;
	void SetPosition(SDL_Point);
};

#endif
