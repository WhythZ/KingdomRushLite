#ifndef _DIRECTION_TYPE_H_
#define _DIRECTION_TYPE_H_

//其中None表示瓦片是无方向的
enum class TileDir
{
	None = 0,
	Up = 1,
	Down = 2,
	Left = 3,
	Right = 4
};

//不存在类似TileDir中None的无意义默认值，仅枚举四个朝向
enum class FacingDir
{
	Up,
	Down,
	Left,
	Right
};

#endif