#ifndef _TOWER_PANEL_H_
#define _TOWER_PANEL_H_

#include <SDL.h>
//以像素点为单位进行定位（坐标均为整型数），无需使用浮点数向量
//#include "../Infra/Vector2.h"

//派生建造轮盘和升级轮盘两类
class TowerPanel
{
protected:
	enum class ButtonType
	{
		None,
		Top,
		Left,
		Right
	};

protected:
	bool isActive = false;                                  //决定当前轮盘UI是否可见
	SDL_Point selectedTileIdx = { 0,0 };                    //当前轮盘UI显示在的瓦片索引坐标
	SDL_Point centerPosition = { 0,0 };                     //轮盘UI中心点的像素坐标位置

	ButtonType hoveredButtonType = ButtonType::None;        //悬停选中的按钮的枚举

	#pragma region Texture
	SDL_Texture* selectedTileCursorTexture = nullptr;       //选中某个瓦片时的光标

	SDL_Texture* idlePanelTexture = nullptr;                //哪个按钮都没选中时，不高亮突出任何按钮
	SDL_Texture* topPanelTexture = nullptr;                 //选中轮盘正上方按钮，高亮突出
	SDL_Texture* leftPanelTexture = nullptr;                //选中轮盘左下方按钮，高亮突出
	SDL_Texture* rightPanelTexture = nullptr;               //选中轮盘右下方按钮，高亮突出
	#pragma endregion

	#pragma region CostValue
	double topCostValue = 0;                                //实时记录正上方按钮对应防御塔花费
	double leftCostValue = 0;                               //实时记录左下方按钮对应防御塔花费
	double rightCostValue = 0;                              //实时记录右下方按钮对应防御塔花费
	#pragma endregion

private:
	#pragma region Setting
	#pragma region Size
	const SDL_Point panelSize = { 144,144 };                //轮盘的整体尺寸

	const SDL_Point buttonSize = { 48,48 };                 //轮盘中各个按钮的尺寸
	const SDL_Point topButtonOffset = { 48,6 };             //正上方按钮偏移轮盘左上顶点的偏移量
	const SDL_Point leftButtonOffset = { 8,80 };            //左下方按钮偏移轮盘左上顶点的偏移量
	const SDL_Point rightButtonOffset = { 90,80 };          //右下方按钮偏移轮盘左上顶点的偏移量

	SDL_Point costTextSize = { 0,0 };
	#pragma endregion

	#pragma region Color
	const SDL_Color costTextColor = { 255,255 ,255 ,255 };
	#pragma endregion
	#pragma endregion
	
	SDL_Texture* costTextTexture = nullptr;

public:
	TowerPanel();
	~TowerPanel();
	void SetSelectedTileIdx(const SDL_Point&);              //获取选定瓦片坐标，同时获取了位置

	void OnInput(const SDL_Event&);                         //监测鼠标的位置输入
	virtual void OnUpdate(SDL_Renderer*);                   //更新文本的纹理渲染数据，故需传入渲染器
	virtual void OnRender(SDL_Renderer*);

	SDL_Point GetSelectedTileIdx() const;
	bool IsActive() const;

	virtual void ShowPanel();                               //显示本轮盘面板
	virtual void ClosePanel();                              //关闭本轮盘面板

protected:
	virtual void OnClickTop() = 0;
	virtual void OnClickLeft() = 0;
	virtual void OnClickRight() = 0;
};

#endif
