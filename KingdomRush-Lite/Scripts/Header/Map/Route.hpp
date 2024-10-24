#ifndef _ROUTE_HPP_
#define _ROUTE_HPP_

#include <vector>
#include "SDL.h"
#include "Tile.hpp"
#include "Map.hpp"

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

Route::Route(const TileMap& _map, const SDL_Point& _beginIdx)
{
	//存储瓦片地图的长宽
	size_t _mapWidth = _map[0].size();
	size_t _mapHeight = _map.size();

	//记录路径的下一个瓦片索引，初始化为传入的起始瓦片
	SDL_Point _nextIdx = _beginIdx;

	//循环
	while (true)
	{
		//若瓦片索引超出了地图边界，则结束循环
		if (_nextIdx.x >= _mapWidth || _nextIdx.y >= _mapHeight)
			break;

		//若路径是否形成闭环，则停止循环，否则放入路径瓦片列表内；第一个被添加进列表的是_beginIdx对应的瓦片索引，之后的点都需要被检验
		if (CheckRepeatedPointIdx(_nextIdx) == true)
			break;
		else
			tilePointList.push_back(_nextIdx);

		//下一个砖块到达了家，那么路径结束
		if (_map[_nextIdx.y][_nextIdx.x].specialFlagLayer == 0)
			break;

		//默认标记下一个瓦片为存在
		bool _isNextTileExist = true;

		//按照瓦片的方向性进行索引的移动
		switch (_map[_nextIdx.y][_nextIdx.x].directionLayer)
		{
		case Tile::TileDir::Up:
			_nextIdx.y--;
			break;
		case Tile::TileDir::Down:
			_nextIdx.y++;
			break;
		case Tile::TileDir::Left:
			_nextIdx.x--;
			break;
		case Tile::TileDir::Right:
			_nextIdx.x++;
			break;
		default:
			//标记下一个路径瓦片为不存在，并结束switch结构
			_isNextTileExist = false;
			break;
		}

		//如果下一个包含有效路径信息的路径瓦片不存在，跳出while循环
		if (!_isNextTileExist)
			break;
	}
}

const Route::TilePointList& Route::GetTilePointList() const
{
	return tilePointList;
}

bool Route::CheckRepeatedPointIdx(const SDL_Point& _targetIdx)
{
	//使用了C++11引入的范围for循环（Range-Based for Loop）进行遍历
	for (const SDL_Point& i : tilePointList)
	{
		//检测坐标重叠
		if (i.x == _targetIdx.x && i.y == _targetIdx.y)
			return true;
	}
	return false;
}

std::ostream& operator<<(std::ostream& _cout, const SDL_Point _pt)
{
	_cout << "(" << _pt.x << "," << _pt.y << ")";
	return _cout;
}

std::ostream& operator<<(std::ostream& _cout, const Route& _route)
{
	Route::TilePointList _pts = _route.GetTilePointList();
	std::cout << "[Begin=" << _pts[0] << ",End=" << _pts[_pts.size() - 1] << "]\n";
	return _cout;
}

#endif
