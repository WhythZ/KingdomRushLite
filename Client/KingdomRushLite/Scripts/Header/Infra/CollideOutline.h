#ifndef _COLLIDE_OUTLINE_H_
#define _COLLIDE_OUTLINE_H_

#include <SDL.h>

//用于可视化绘制碰撞向的组件
class CollideOutline
{
public:
	//额外传入碰撞体的渲染颜色、中心坐标、方形尺寸大小、旋转角度
	void DrawBoxOutline(SDL_Renderer*, SDL_Color, SDL_Point, SDL_Point, double);
};

#endif
