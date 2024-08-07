#ifndef _TILE_H_
#define _TILE_H_

#include <vector>

//宏定义每个瓦片的像素大小是48个像素
#define TILE_SIZE 48

//地图砖块的结构体（无方法的纯数据结构习惯用结构体），每个砖块用四个图层变量的值确定自身性质
struct Tile
{
	enum class TileDir
	{
		None = 0,
		Up = 1,
		Down = 2,
		Left = 3,
		Right = 4
	};

	//砖块最底层的地形渲染图层；
	int terrainLayer = 0;

	//砖块第二层的装饰渲染图层；
	int decorationLayer = 0;
	
	//砖块第三层的怪物行进路线渲染图层；None表示此砖快不是怪物的行进路线砖块
	TileDir directionLayer = TileDir::None;
	
	//砖块最上层的特殊事物渲染图层；-1是无特殊事物，0是家，1、2、3等整数分别表示不同的刷怪点
	int specialLayer = -1;
};

//自定义数据类型TileMap，是一个存储Tile元素的二维vector容器数组
typedef std::vector<std::vector<Tile>> TileMap;

#endif