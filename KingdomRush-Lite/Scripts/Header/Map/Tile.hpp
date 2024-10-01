#ifndef _TILE_HPP_
#define _TILE_HPP_

#include <vector>

//�궨��ÿ����Ƭ�����ش�С��48������
#define TILE_SIZE 48

//��ͼ��Ƭ�Ľṹ�壨�޷����Ĵ����ݽṹϰ���ýṹ�壩��ÿ����Ƭ���ĸ�ͼ������Լ�һ���жϷ��������ڵĲ���ֵȷ����������
struct Tile
{
	enum class TileDir
	{
		None = 0,
		Up = 1,
		Down = 2,
		Left = 3,
		Right = 4
	};

	int terrainLayer = 0;                    //��Ƭ��ײ�ĵ�����Ⱦͼ�㣻ÿ���Ǹ�������ʾһ�ֵ��Σ�С��0������
	int decorationLayer = -1;                //��Ƭ�ڶ����װ����Ⱦͼ�㣻ÿ���Ǹ�������ʾһ��װ�Σ�-1��ʾ��װ��
	TileDir directionLayer = TileDir::None;  //��Ƭ������Ĺ����н�·����Ⱦͼ�㣻None��ʾ����Ƭ���ǹ�����н�·����Ƭ��С��0������
	int specialFlagLayer = -1;               //��Ƭ���ϲ������������Ⱦͼ�㣻0�Ǽң�1��2��3�������ֱ��ʾ��ͬ��ˢ�ֵ㣬-1��ʾ����������
	bool hasTower = false;                   //����һ������ʱ������жϵ����ݣ����÷�����ʱ���жϵ�ǰ��Ƭ�Ƿ��Ѿ����ڷ�����
};

#endif