#ifndef _DIRECTION_TYPE_H_
#define _DIRECTION_TYPE_H_

//����None��ʾ��Ƭ���޷����
enum class TileDir
{
	None = 0,
	Up = 1,
	Down = 2,
	Left = 3,
	Right = 4
};

//����������TileDir��None��������Ĭ��ֵ����ö���ĸ�����
enum class FacingDir
{
	Up,
	Down,
	Left,
	Right
};

#endif