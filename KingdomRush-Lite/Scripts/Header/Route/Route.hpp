#ifndef _ROUTE_HPP_
#define _ROUTE_HPP_

#include "SDL.h"
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
		size_t mapWidth = _map[0].size();
		size_t mapHeight = _map.size();
	}
};

#endif
