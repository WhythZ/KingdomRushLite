#ifndef _MAP_HPP_
#define _MAP_HPP_

//һ�ֹ�ϣ�������Լ�ֵ�Ե���ʽ�洢��ɢ����
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include "SDL.h"
#include "Route.hpp"

//·����
typedef std::unordered_map<int, Route> RoutePool;

//�ؿ���ͼ
class Map
{
private:
	TileMap tileMap;                                    //��ǰ�ؿ���ͼ����Ƭ��ͼ
	SDL_Point homePoint = { 0 };                        //�洢�ң����صص㣩����Ƭ��ͼ�е�λ������
	RoutePool spawnRoutePool;                           //ˢ�ֵ�·���أ��洢�����ɢ��Route����ͼ

public:
	Map() = default;
	~Map() = default;

	bool Load(const std::string&);                      //��ָ��·��������Ƭ��ͼCSV�ļ��������سɹ����
	void MarkTowerExistenceAt(const SDL_Point&);        //��Ƿ��������ض���Ƭ��
	const TileMap& GetTileMap() const;                  //��ȡ��Ƭ��ͼ�ĳ�����
	const SDL_Point& GetHomePoint() const;              //��ȡ����Ƭ��λ�ó�����
	const RoutePool& GetSpawnRoutePool() const;         //��ȡˢ�ֵ�·���صĳ�����
	size_t GetHeightTileNum() const;                    //��ȡ��ͼ�ĸߣ�����Ƭ����
	size_t GetWidthTileNum() const;                     //��ȡ��ͼ�Ŀ�����Ƭ����

private:
	std::string TrimString(const std::string);          //������Ƭ�ַ����Ա�֤��ʽ��ͳһ�Ա��ȡ
	void LoadTileFromString(const std::string, Tile&);  //����Ƭ�ַ�������ȡ��Ϣ������
	void GenerateMapCache();                            //���ڶ�ȡ���ɵ�ͼ�Ļ���
};

#endif