#ifndef _TILE_H_
#define _TILE_H_

#include <vector>
#include "../Infra/DirectionType.h"
#include "../Tower/TowerType.h"

//宏定义每个瓦片的像素大小是48个像素
#define TILE_SIZE 48

//地图瓦片的结构体（无方法的纯数据结构习惯用结构体），每个瓦片用四个图层变量以及一个判断防御塔存在的布尔值确定自身性质
struct Tile
{
	#pragma region LoadFromCSV
	int terrainLayer = 0;                    //瓦片最底层的地形渲染图层；每个非负整数表示一种地形，小于0无意义
	int decorationLayer = -1;                //瓦片第二层的装饰渲染图层；每个非负整数表示一种装饰，-1表示无装饰
	TileDir directionLayer = TileDir::None;  //瓦片第三层的怪物行进路线渲染图层；None表示此瓦片不是怪物的行进路线瓦片
	int specialFlagLayer = -1;               //瓦片最上层的特殊事物渲染图层；0是家，1、2、3等整数分别表示不同的刷怪点，-1表示无特殊事物
	#pragma endregion

	bool hasTower = false;                   //这是一个运行时需进行判断的数据；放置防御塔时需判断当前瓦片是否已经存在防御塔
	TowerType towerType = TowerType::None;   //存储当前瓦片上防御塔的类型，若无防御塔则为None
};

//自定义数据类型TileMap，是一个存储Tile元素的二维vector容器数组
typedef std::vector<std::vector<Tile>> TileMap;

#endif