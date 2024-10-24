#ifndef _ROUTE_HPP_
#define _ROUTE_HPP_

#include <vector>
#include "SDL.h"
#include "Tile.hpp"
#include "Map.hpp"

//������н�·������ͼ
class Route
{
public:
	//�洢�н�·������Ƭ��Ķ�ά�����������������
	typedef std::vector<SDL_Point> TilePointList;

private:
	TilePointList tilePointList;                   //�������ڴ洢�н�·������Ƭ��ά������

public:
	Route(const TileMap&, const SDL_Point&);       //������Ƭ��ͼ����㵥Ԫ��λ�ã����ɹ����н�·����Ƭ�����б�
	Route() = default; 
	~Route() = default;

	const TilePointList& GetTilePointList() const; //�����ṩ��ȡ·����Ƭ�����б����õ�ֻ���ӿ�

private:
	bool CheckRepeatedPointIdx(const SDL_Point&);  //��鴫�����Ƭ�����Ƿ��Ѿ���·����Ƭ�б��г��ֹ�����ֹ·����β�����ɹ��캯������ѭ��
};

Route::Route(const TileMap& _map, const SDL_Point& _beginIdx)
{
	//�洢��Ƭ��ͼ�ĳ���
	size_t _mapWidth = _map[0].size();
	size_t _mapHeight = _map.size();

	//��¼·������һ����Ƭ��������ʼ��Ϊ�������ʼ��Ƭ
	SDL_Point _nextIdx = _beginIdx;

	//ѭ��
	while (true)
	{
		//����Ƭ���������˵�ͼ�߽磬�����ѭ��
		if (_nextIdx.x >= _mapWidth || _nextIdx.y >= _mapHeight)
			break;

		//��·���Ƿ��γɱջ�����ֹͣѭ�����������·����Ƭ�б��ڣ���һ������ӽ��б����_beginIdx��Ӧ����Ƭ������֮��ĵ㶼��Ҫ������
		if (CheckRepeatedPointIdx(_nextIdx) == true)
			break;
		else
			tilePointList.push_back(_nextIdx);

		//��һ��ש�鵽���˼ң���ô·������
		if (_map[_nextIdx.y][_nextIdx.x].specialFlagLayer == 0)
			break;

		//Ĭ�ϱ����һ����ƬΪ����
		bool _isNextTileExist = true;

		//������Ƭ�ķ����Խ����������ƶ�
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

#endif
