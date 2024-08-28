#ifndef _ROUTE_HPP_
#define _ROUTE_HPP_

#include <vector>
#include "SDL.h"
#include "../Map/Tile.h"
#include "../Map/Map.hpp"

//记录怪物的行进路径，用于生成洋流图
class Route
{
private:
	//存储行进路径上瓦片索引点的数组数据类型
	typedef std::vector<SDL_Point> TileIdxList;
	TileIdxList tileIdxList;

public:
	//默认的无参构造
	Route() = default;
	
	//传入瓦片地图进行怪物行进路径瓦片索引列表的生成，以第二参数传入的索引指定的单元格位置为起点开始寻路（怪物可能有多个出生点）
	Route(const TileMap& _map, const SDL_Point& _beginIdx)
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
				tileIdxList.push_back(_nextIdx);

			//下一个砖块到达了家，那么路径结束
			if (_map[_nextIdx.y][_nextIdx.x].specialLayer == 0)
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
	
	~Route() = default;

	//对外提供获取路径瓦片索引列表常引用的接口
	const TileIdxList& GetTileIdxList()
	{
		return tileIdxList;
	}

private:
	//检查传入的瓦片索引是否已经在路径瓦片列表中出现过，防止路径首尾相接造成构造函数的死循环
	bool CheckRepeatedPointIdx(const SDL_Point& _targetIdx)
	{
		//使用了C++11引入的范围for循环（Range-Based for Loop）进行遍历
		for (const SDL_Point& i : tileIdxList)
		{
			//检测坐标重叠
			if (i.x == _targetIdx.x && i.y == _targetIdx.y)
				return true;
		}
		return false;
	}
};

#endif
