#ifndef _MAP_HPP_
#define _MAP_HPP_

#include <string>
#include <fstream>
#include <sstream>

#include "SDL.h"
#include "Tile.hpp"

//�ؿ���ͼ
class Map
{
public:
	Map() = default;
	~Map() = default;

public:
	//��ָ��·�����ض�ȡ��Ƭ��ͼ��CSV�ļ��������ز���ֵ��ʾ���سɹ����
	bool Load(const std::string& _csvPath)
	{
		//�����ļ������жϸ�·���ļ��Ƿ��ܴ�
		std::ifstream file(_csvPath);
		if (!file.good()) return false;

		//��ʱ����Ƭ��ͼ�����Ƕ�ȡʧ�������踳ֵ����Ա��������Ƭ��ͼ
		TileMap tileMapTemp;
		//��¼��ǰÿ������ȡ����ƬӦ������tileMapTemp���ĸ�����λ�ã�yΪ��������xΪ������
		int xIdx = -1, yIdx = -1;

		//���н���ʵ�ʵ�ͼ���ȡ��ÿ�к��ж����Ƭ
		std::string lineBuf;
		//ÿ��ѭ���ֱ��ļ���ĳ�ж�ȡ��ŵ�lineBuf
		while (std::getline(file, lineBuf))
		{
			//��һ������Ƭ�����ַ�������ͳһ���޼�
			lineBuf = TrimString(lineBuf);
			//�����һ��Ϊ�գ���ô������һ��
			if (lineBuf.empty())
				continue;

			//����������ʼ��Ϊ0�����ӵ�һ�п�ʼ����ÿ��һ�ж�������Զ�λ��һ��
			yIdx++;
			//std::vector<std::vector<Tile>>��Ȧ��vector����һ��Ԫ��λ��ʵ����������ʱ��ͼ������һ��Ԥ������line�����ݶ��룩
			tileMapTemp.emplace_back();

			//��ÿ�е�����ת��Ϊ������Ȼ�޷���getline������ȡ
			std::stringstream lineBufStream(lineBuf);
			//�洢ÿ����Ƭ�ַ�������
			std::string tileBuf;
			//�Զ���Ϊ�����һ���ַ����ж�ȡ������Ƭ�ַ������ݣ�"x\x\x\x"��ʽ�����ÿ����Ƭ�ַ����������ݽ���
			while (std::getline(lineBufStream, tileBuf, ','))
			{
				//��һ��ѭ��ʱ�������걻��ʼ��Ϊ-1+1=0����λ����һ�е�һ�е�λ�ã�ÿ��ȡһ����Ƭ�������궼������Զ�λ��һ��Ҳ����һ����Ƭ
				xIdx++;
				//std::vector<std::vector<Tile>>��Ȧ��vector����һ��Ԫ��λ���ڴ洢�µ���Ƭ���ݶ���
				tileMapTemp[yIdx].emplace_back();

				//��ȡtileBuf�ַ����е���Ƭ���ݲ�������ʱ��ͼ��Ӧ��Ƭ�ϣ�Ҳ������tileMapTemp[yIdx].back()
				Tile& tile = tileMapTemp[yIdx][xIdx];
				LoadTileFromString(tileBuf, tile);
			}
		}

		//�ر��ļ�
		file.close();
		//�Ա�������ɵ���ʱ��ͼ���м��飬�˴�����ȫ�յĵ�ͼ���ߵ�һ��Ϊ�յĵ�ͼ����false��ʾ����ʧ��
		if (tileMapTemp.empty() || tileMapTemp[0].empty())
			return false;
		//���سɹ��Ļ��Ϳ��Է��ĸ�ֵ��������true
		tileMap = tileMapTemp;
		return true;

		//���ɵ�ͼ����
		GenerateMapCache();
	}

	//��ȡ��ͼ�ĸߣ���������ע�ⷵ��ֵ�����Լ�const����
	size_t GetHeight() const
	{
		if (tileMap.empty())
			return 0;
		//�������е�����
		return tileMap.size();
	}

	//��ȡ��ͼ�Ŀ�������
	size_t GetWidth() const
	{
		if (tileMap.empty())
			return 0;
		//�����ص�һ�е�����
		return tileMap[0].size();
	}

private:
	//��ǰ�ؿ���ͼ����Ƭ��ͼ
	TileMap tileMap;
	
	//ʹ��SDL��ĵ����洢�ң����صص㣩����Ƭ������λ�ã������ڱ���׶λ���TileMap�д洢�ľ�̬���ݵķ�����Ϊ����
	SDL_Point homeIdx = { 0 };

private:
	//���ַ�������β�����˿ո񣬻���ѱ����֣������ṩһ�����в�����ͳһ����"   x\x\x\x,y\y\y\y   "�����к�õ�"x\x\x\x.y\y\y\y"
	std::string TrimString(const std::string _str)
	{
		//�ҵ��ַ����еĵ�һ���ǿ��ַ����ǿո񡢷��Ʊ������������ע��ʹ�õ���size_t����
		size_t beginIdx = _str.find_first_not_of(" \t");
		//���û���ҵ�����ô���ؿ��ַ���
		if (beginIdx == std::string::npos)
			return "";
		//�ҵ��ַ����е����һ���ǿ��ַ�����
		size_t endIdx = _str.find_last_not_of(" \t");

		//�������ַ����ַ����ĳ���
		size_t idxRange = endIdx - beginIdx + 1;
		//���شӵ�һ���ǿ��ַ������������ȵ��ַ���
		return _str.substr(beginIdx, idxRange);
	}

	//����Ƭ�ַ�������ȡ��Ϣ������
	void LoadTileFromString(const std::string _tileBuf, Tile& _tile)
	{
		//������ַ���������β���ַ���ȥ��
		std::string trimmedTileBuf = TrimString(_tileBuf);

		//��һ��������ʱ�洢����������������
		std::vector<int> values;

		//������Ƭ������Ϣ��¼�룻ע��'\\'��ת���ַ�
		std::stringstream tileBufStream(trimmedTileBuf);
		std::string valueBuf;
		while (std::getline(tileBufStream, valueBuf, '\\'))
		{
			//��ʱ���ڴ洢��ȡ������Ƭ��Ա����Ϣ
			int value;

			//���쳣�Ļ���������������ת��
			try
			{
				//�ַ���ת����
				value = std::stoi(valueBuf);
			}
			//�����׳���std::invalid_argument&�쳣�������˷���ֵ�ַ�������ִ�������ڲ������
			catch (const std::invalid_argument&)
			{
				//����Ƭ��Ա������ȡֵ�У�-1Ҫô��������ֵ��Ҫô��Ĭ��ֵ
				value = -1;
			}

			//����ȡ������ֵ�Ⱥ��������
			values.push_back(value);
		}

		//�������ߴ磬�յĻ���ʾ����ȱʧ�����β��ֵС��0�����壬����0��ʾȡĬ�ϵ���
		_tile.terrainLayer = (values.size() < 1 || values[0] < 0) ? 0 : values[0];
		//����ߴ�Ϊ1�Ļ���˵�����˵��β�������������������ݾ�ȱʧ�������������ƣ�С��-1�����壬����-1��ʾ��װ��
		_tile.decorationLayer = (values.size() < 2 || values[0] < -1) ? -1 : values[1];
		//����ö���ڵ�ӳ�����ǿ������ת����С��0�����壬0��ӦTile::TileDir::None����ʾ�޷���
		_tile.directionLayer = (Tile::TileDir)((values.size() < 3 || values[0] < 0) ? 0 : values[2]);
		//С��-1�����壬����-1��ʾ�����⽨��
		_tile.specialLayer = (values.size() < 4 || values[0] < -1) ? -1 : values[3];
	}

	//���ɵ�ͼ�Ļ���
	void GenerateMapCache()
	{
		//������ͼ��ÿһ����Ƭ��Ԫ��
		for (int y = 0; y < GetHeight(); y++)
		{
			for (int x = 0; x < GetWidth(); x++)
			{
				//��ʱʹ��һ��������Ƭ���ô洢
				const Tile& tile = tileMap[y][x];
				if (tile.specialLayer < 0)
					continue;

				//�洢�ҵ�����
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