#ifndef _ROUTE_HPP_
#define _ROUTE_HPP_

#include <vector>
#include "SDL.h"
#include "../Map/Tile.h"
#include "../Map/Map.hpp"

//��¼������н�·����������������ͼ
class Route
{
private:
	//�洢�н�·������Ƭ�������������������
	typedef std::vector<SDL_Point> TileIdxList;
	TileIdxList tileIdxList;

public:
	//Ĭ�ϵ��޲ι���
	Route() = default;
	
	//������Ƭ��ͼ���й����н�·����Ƭ�����б�����ɣ��Եڶ��������������ָ���ĵ�Ԫ��λ��Ϊ��㿪ʼѰ·����������ж�������㣩
	Route(const TileMap& _map, const SDL_Point& _beginIdx)
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
				tileIdxList.push_back(_nextIdx);

			//��һ��ש�鵽���˼ң���ô·������
			if (_map[_nextIdx.y][_nextIdx.x].specialLayer == 0)
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
	
	~Route() = default;

	//�����ṩ��ȡ·����Ƭ�����б����õĽӿ�
	const TileIdxList& GetTileIdxList()
	{
		return tileIdxList;
	}

private:
	//��鴫�����Ƭ�����Ƿ��Ѿ���·����Ƭ�б��г��ֹ�����ֹ·����β�����ɹ��캯������ѭ��
	bool CheckRepeatedPointIdx(const SDL_Point& _targetIdx)
	{
		//ʹ����C++11����ķ�Χforѭ����Range-Based for Loop�����б���
		for (const SDL_Point& i : tileIdxList)
		{
			//��������ص�
			if (i.x == _targetIdx.x && i.y == _targetIdx.y)
				return true;
		}
		return false;
	}
};

#endif
