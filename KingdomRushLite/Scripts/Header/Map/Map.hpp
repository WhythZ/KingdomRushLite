#ifndef _MAP_HPP_
#define _MAP_HPP_

//一种哈希表，用于以键值对的形式存储离散数据
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include "SDL.h"
#include "Route.hpp"

//路径池
typedef std::unordered_map<int, Route> RoutePool;

//关卡地图
class Map
{
private:
	TileMap tileMap;                                    //当前关卡地图的瓦片地图
	SDL_Point homePoint = { 0 };                        //存储家（防守地点）在瓦片地图中的位置索引
	RoutePool spawnRoutePool;                           //刷怪点路径池，存储多个离散的Route洋流图

public:
	Map() = default;
	~Map() = default;

	bool Load(const std::string&);                      //从指定路径加载瓦片地图CSV文件，并返回成功与否
	void MarkTowerExistenceAt(const SDL_Point&);        //标记防御塔在特定瓦片处
	const TileMap& GetTileMap() const;                  //获取瓦片地图的常引用
	const SDL_Point& GetHomePoint() const;              //获取家瓦片的位置常引用
	const RoutePool& GetSpawnRoutePool() const;         //获取刷怪点路径池的常引用
	size_t GetHeightTileNum() const;                    //获取地图的高（行瓦片数）
	size_t GetWidthTileNum() const;                     //获取地图的宽（列瓦片数）

private:
	std::string TrimString(const std::string);          //剪切瓦片字符串以保证格式的统一以便读取
	void LoadTileFromString(const std::string, Tile&);  //从瓦片字符串中提取信息并载入
	void GenerateMapCache();                            //用于读取生成地图的缓存
};

#endif