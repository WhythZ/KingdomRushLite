#include "../../Header/Map/Route.h"
#include <iostream>

Route::Route(const TileMap& _map, const SDL_Point& _beginPoint)
{
	//�洢��Ƭ��ͼ�ĳ���
	size_t _mapWidth = _map[0].size();
	size_t _mapHeight = _map.size();

	//��¼·������һ����Ƭ��������ʼ��Ϊ�������ʼ��Ƭ
	SDL_Point _nextPoint = _beginPoint;

	//ѭ��
	while (true)
	{
		//����Ƭ���������˵�ͼ�߽磬�����ѭ��
		if (_nextPoint.x >= _mapWidth || _nextPoint.y >= _mapHeight)
			break;

		//��·���γɱջ�����ֹͣѭ�����������·����Ƭ�б��ڣ���һ������ӽ��б����_beginIdx��Ӧ����Ƭ������֮��ĵ㶼��Ҫ������
		if (CheckRepeatedPointIdx(_nextPoint))
			break;
		else
			tilePointList.push_back(_nextPoint);

		//std::cout << "tilePointList.size()=" << tilePointList.size() << " _nexPoint=(" << _nextPoint.x << "," << _nextPoint.y << ")\n";

		//��һ��ש�鵽���˼ң���ô·������
		if (_map[_nextPoint.y][_nextPoint.x].specialFlagLayer == 0)
			break;

		//Ĭ�ϱ����һ����ƬΪ����
		bool _isNextTileExist = true;

		//������Ƭ�ķ����Խ����������ƶ�
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
			//�����һ��·����ƬΪ�����ڣ�������switch�ṹ
			_isNextTileExist = false;
			break;
		}

		//�����һ��������Ч·����Ϣ��·����Ƭ�����ڣ�����whileѭ��
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
	//ʹ����C++11����ķ�Χforѭ����Range-Based for Loop�����б���
	for (const SDL_Point& i : tilePointList)
	{
		//��������ص�
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