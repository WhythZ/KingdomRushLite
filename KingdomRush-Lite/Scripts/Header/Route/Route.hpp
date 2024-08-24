#ifndef _ROUTE_HPP_
#define _ROUTE_HPP_

#include "SDL.h"
#include "../Map/Map.hpp"

//记录怪物的行进路径，用于生成洋流图
class Route
{
public:
	//默认的无参构造
	Route() = default;
	//传入瓦片地图进行怪物行进路径的获取，以第二参数传入的索引指定的单元格位置为起点开始寻路（怪物可能有多个出生点）
	Route(const TileMap& _map, const SDL_Point& _beginIdx)
	{
		//存储瓦片地图的长宽
		size_t mapWidth = _map[0].size();
		size_t mapHeight = _map.size();
	}
};

#endif
