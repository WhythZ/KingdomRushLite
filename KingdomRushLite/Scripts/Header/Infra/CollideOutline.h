#ifndef _COLLIDE_OUTLINE_H_
#define _COLLIDE_OUTLINE_H_

#include <SDL.h>

//���ڿ��ӻ�������ײ������
class CollideOutline
{
public:
	//���⴫����ײ�����Ⱦ��ɫ���������ꡢ���γߴ��С����ת�Ƕ�
	void DrawBoxOutline(SDL_Renderer*, SDL_Color, SDL_Point, SDL_Point, double);
};

#endif
