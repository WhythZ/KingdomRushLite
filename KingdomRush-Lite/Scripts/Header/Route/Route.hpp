////
////·����
////

#ifndef _ROUTE_HPP_
#define _ROUTE_HPP_

#include "SDL.h"
#include "../Map/Tile.h"
#include "../Map/Map.hpp"

//��¼������н�·����������������ͼ
class Route
{
public:
	//Ĭ�ϵ��޲ι���
	Route() = default;
	//������Ƭ��ͼ���й����н�·���Ļ�ȡ���Եڶ��������������ָ���ĵ�Ԫ��λ��Ϊ��㿪ʼѰ·����������ж�������㣩
	Route(const TileMap& _map, const SDL_Point& _beginIdx)
	{
		//�洢��Ƭ��ͼ�ĳ���
		size_t _mapWidth = _map[0].size();
		size_t _mapHeight = _map.size();
	
		//
		SDL_Point _nextIdx = _beginIdx;
	}
};

#endif
