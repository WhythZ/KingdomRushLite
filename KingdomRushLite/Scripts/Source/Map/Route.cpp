#include "../../Header/Map/Route.h"
#include <iostream>

Route::Route(const TileMap& _map, const SDL_Point& _beginPoint)
{
	//存储瓦片地图的长宽
	size_t _mapWidth = _map[0].size();
	size_t _mapHeight = _map.size();

	//记录路径的下一个瓦片索引，初始化为传入的起始瓦片
	SDL_Point _nextPoint = _beginPoint;

	//循环
	while (true)
	{
		//若瓦片索引超出了地图边界，则结束循环
		if (_nextPoint.x >= _mapWidth || _nextPoint.y >= _mapHeight)
			break;

		//若路径形成闭环，则停止循环，否则放入路径瓦片列表内；第一个被添加进列表的是_beginIdx对应的瓦片索引，之后的点都需要被检验
		if (CheckRepeatedPointIdx(_nextPoint))
			break;
		else
			tilePointList.push_back(_nextPoint);

		//std::cout << "tilePointList.size()=" << tilePointList.size() << " _nexPoint=(" << _nextPoint.x << "," << _nextPoint.y << ")\n";

		//下一个砖块到达了家，那么路径结束
		if (_map[_nextPoint.y][_nextPoint.x].specialFlagLayer == 0)
			break;

		//默认标记下一个瓦片为存在
		bool _isNextTileExist = true;

		//按照瓦片的方向性进行索引的移动
		switch (_map[_nextPoint.y][_nextPoint.x].directionLayer)
		{
		case TileDir::Up:
			_nextPoint.y--;
			break;
		case TileDir::Down:
			_nextPoint.y++;
			break;
		case TileDir::Left:
			_nextPoint.x--;
			break;
		case TileDir::Right:
			_nextPoint.x++;
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