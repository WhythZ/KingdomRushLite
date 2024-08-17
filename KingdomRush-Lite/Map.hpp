#ifndef _MAP_HPP_
#define _MAP_HPP_

#include <string>
#include <fstream>
#include <sstream>

#include "SDL.h"
#include "Tile.hpp"

//关卡地图
class Map
{
public:
	Map() = default;
	~Map() = default;

public:
	//从指定路径加载读取瓦片地图的CSV文件，并返回布尔值表示加载成功与否
	bool Load(const std::string& _csvPath)
	{
		//载入文件，并判断该路径文件是否能打开
		std::ifstream file(_csvPath);
		if (!file.good()) return false;

		//临时的瓦片地图，若是读取失败则无需赋值给成员变量的瓦片地图
		TileMap tileMapTemp;
		//记录当前每个被读取的瓦片应被读入tileMapTemp的哪个索引位置，y为行索引，x为列索引
		int xIdx = -1, yIdx = -1;

		//逐行进行实际的图格读取，每行含有多个瓦片
		std::string lineBuf;
		//每次循环分别将文件的某行读取存放到lineBuf
		while (std::getline(file, lineBuf))
		{
			//将一整行瓦片数据字符串进行统一化修剪
			lineBuf = TrimString(lineBuf);
			//如果这一行为空，那么跳过这一行
			if (lineBuf.empty())
				continue;

			//将行索引初始化为0，即从第一行开始读，每读一行都会递增以定位下一行
			yIdx++;
			//std::vector<std::vector<Tile>>外圈的vector增加一个元素位（实际意义是临时地图开辟了一行预备进行line的数据读入）
			tileMapTemp.emplace_back();

			//将每行的数据转化为流，不然无法被getline函数读取
			std::stringstream lineBufStream(lineBuf);
			//存储每个瓦片字符串数据
			std::string tileBuf;
			//以逗号为间隔从一行字符串中读取单个瓦片字符串数据（"x\x\x\x"形式）后对每个瓦片字符串进行数据解析
			while (std::getline(lineBufStream, tileBuf, ','))
			{
				//第一次循环时，列坐标被初始化为-1+1=0，定位到第一行第一列的位置，每读取一个瓦片，列坐标都会递增以定位下一列也即下一个瓦片
				xIdx++;
				//std::vector<std::vector<Tile>>内圈的vector增加一个元素位用于存储新的瓦片数据读入
				tileMapTemp[yIdx].emplace_back();

				//读取tileBuf字符串中的瓦片数据并载入临时地图对应瓦片上；也可以用tileMapTemp[yIdx].back()
				Tile& tile = tileMapTemp[yIdx][xIdx];
				LoadTileFromString(tileBuf, tile);
			}
		}

		//关闭文件
		file.close();
		//对被加载完成的临时地图进行检验，此处对于全空的地图或者第一行为空的地图返回false表示加载失败
		if (tileMapTemp.empty() || tileMapTemp[0].empty())
			return false;
		//加载成功的话就可以放心赋值，并返回true
		tileMap = tileMapTemp;
		return true;

		//生成地图缓存
		GenerateMapCache();
	}

	//获取地图的高（行数），注意返回值类型以及const修饰
	size_t GetHeight() const
	{
		if (tileMap.empty())
			return 0;
		//即返回行的数量
		return tileMap.size();
	}

	//获取地图的宽（列数）
	size_t GetWidth() const
	{
		if (tileMap.empty())
			return 0;
		//即返回第一行的列数
		return tileMap[0].size();
	}

private:
	//当前关卡地图的瓦片地图
	TileMap tileMap;
	
	//使用SDL库的点来存储家（防守地点）的瓦片的索引位置；这种在编译阶段缓存TileMap中存储的静态数据的方法称为烘培
	SDL_Point homeIdx = { 0 };

private:
	//若字符串的首尾出现了空格，会很难被发现，所以提供一个剪切操作以统一，如"   x\x\x\x,y\y\y\y   "被剪切后得到"x\x\x\x.y\y\y\y"
	std::string TrimString(const std::string _str)
	{
		//找到字符串中的第一个非空字符（非空格、非制表符）的索引；注意使用的是size_t类型
		size_t beginIdx = _str.find_first_not_of(" \t");
		//如果没有找到，那么返回空字符串
		if (beginIdx == std::string::npos)
			return "";
		//找到字符串中的最后一个非空字符索引
		size_t endIdx = _str.find_last_not_of(" \t");

		//不含空字符的字符串的长度
		size_t idxRange = endIdx - beginIdx + 1;
		//返回从第一个非空字符算起、上述长度的字符串
		return _str.substr(beginIdx, idxRange);
	}

	//从瓦片字符串中提取信息并载入
	void LoadTileFromString(const std::string _tileBuf, Tile& _tile)
	{
		//对这个字符串进行首尾空字符的去除
		std::string trimmedTileBuf = TrimString(_tileBuf);

		//用一个数组临时存储被解析出来的数据
		std::vector<int> values;

		//进行瓦片各层信息的录入；注意'\\'是转义字符
		std::stringstream tileBufStream(trimmedTileBuf);
		std::string valueBuf;
		while (std::getline(tileBufStream, valueBuf, '\\'))
		{
			//临时用于存储读取到的瓦片成员的信息
			int value;

			//无异常的话就正常进行类型转换
			try
			{
				//字符串转整型
				value = std::stoi(valueBuf);
			}
			//若是抛出了std::invalid_argument&异常（出现了非数值字符），就执行以下内部的语句
			catch (const std::invalid_argument&)
			{
				//在瓦片成员变量的取值中，-1要么是无意义值，要么是默认值
				value = -1;
			}

			//将读取到的数值先后存入数组
			values.push_back(value);
		}

		//检测数组尺寸，空的话表示数据缺失；地形层的值小于0无意义，等于0表示取默认地形
		_tile.terrainLayer = (values.size() < 1 || values[0] < 0) ? 0 : values[0];
		//数组尺寸为1的话，说明除了地形层有数据外其他层的数据均缺失，后面依此类推；小于-1无意义，等于-1表示无装饰
		_tile.decorationLayer = (values.size() < 2 || values[0] < -1) ? -1 : values[1];
		//依据枚举内的映射进行强制类型转换；小于0无意义，0对应Tile::TileDir::None，表示无方向
		_tile.directionLayer = (Tile::TileDir)((values.size() < 3 || values[0] < 0) ? 0 : values[2]);
		//小于-1无意义，等于-1表示无特殊建筑
		_tile.specialLayer = (values.size() < 4 || values[0] < -1) ? -1 : values[3];
	}

	//生成地图的缓存
	void GenerateMapCache()
	{
		//遍历地图的每一个瓦片单元格
		for (int y = 0; y < GetHeight(); y++)
		{
			for (int x = 0; x < GetWidth(); x++)
			{
				//临时使用一个常量瓦片引用存储
				const Tile& tile = tileMap[y][x];
				if (tile.specialLayer < 0)
					continue;

				//存储家的坐标
				if (tile.specialLayer == 0)
				{
					homeIdx.x = x;
					homeIdx.y = y;
				}
			}
		}
	}
};

#endif