#ifndef _ROUTE_HPP_
#define _ROUTE_HPP_

#include <vector>
#include "SDL.h"
#include "Tile.hpp"

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

#endif
