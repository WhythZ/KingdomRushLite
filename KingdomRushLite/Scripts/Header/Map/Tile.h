#ifndef _TILE_H_
#define _TILE_H_

#include <vector>
#include "../Infra/DirectionType.h"
#include "../Tower/TowerType.h"

//�궨��ÿ����Ƭ�����ش�С��48������
#define TILE_SIZE 48

//��ͼ��Ƭ�Ľṹ�壨�޷����Ĵ����ݽṹϰ���ýṹ�壩��ÿ����Ƭ���ĸ�ͼ������Լ�һ���жϷ��������ڵĲ���ֵȷ����������
struct Tile
{
	#pragma region LoadFromCSV
	int terrainLayer = 0;                    //��Ƭ��ײ�ĵ�����Ⱦͼ�㣻ÿ���Ǹ�������ʾһ�ֵ��Σ�С��0������
	int decorationLayer = -1;                //��Ƭ�ڶ����װ����Ⱦͼ�㣻ÿ���Ǹ�������ʾһ��װ�Σ�-1��ʾ��װ��
	TileDir directionLayer = TileDir::None;  //��Ƭ������Ĺ����н�·����Ⱦͼ�㣻None��ʾ����Ƭ���ǹ�����н�·����Ƭ
	int specialFlagLayer = -1;               //��Ƭ���ϲ������������Ⱦͼ�㣻0�Ǽң�1��2��3�������ֱ��ʾ��ͬ��ˢ�ֵ㣬-1��ʾ����������
	#pragma endregion

	bool hasTower = false;                   //����һ������ʱ������жϵ����ݣ����÷�����ʱ���жϵ�ǰ��Ƭ�Ƿ��Ѿ����ڷ�����
	TowerType towerType = TowerType::None;   //�洢��ǰ��Ƭ�Ϸ����������ͣ����޷�������ΪNone
};

//�Զ�����������TileMap����һ���洢TileԪ�صĶ�άvector��������
typedef std::vector<std::vector<Tile>> TileMap;

#endif