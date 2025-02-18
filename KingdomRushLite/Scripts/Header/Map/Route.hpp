#ifndef _ROUTE_HPP_
#define _ROUTE_HPP_

#include <vector>
#include "SDL.h"
#include "Tile.hpp"

//怪物的行进路径洋流图
class Route
{
public:
	//存储行进路径上瓦片点的二维坐标的数组数据类型
	typedef std::vector<SDL_Point> TilePointList;

private:
	TilePointList tilePointList;                   //数组用于存储行进路径上瓦片二维点坐标

public:
	Route(const TileMap&, const SDL_Point&);       //传入瓦片地图和起点单元格位置，生成怪物行进路径瓦片索引列表
	Route() = default; 
	~Route() = default;

	const TilePointList& GetTilePointList() const; //对外提供获取路径瓦片索引列表常引用的只读接口

private:
	bool CheckRepeatedPointIdx(const SDL_Point&);  //检查传入的瓦片索引是否已经在路径瓦片列表中出现过，防止路径首尾相接造成构造函数的死循环
};

#endif
