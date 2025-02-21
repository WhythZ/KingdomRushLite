#ifndef _UI_MANAGER_H_
#define _UI_MANAGER_H_

#include "../Manager.hpp"
#include <SDL.h>
#include "../../UI/StatusUI.h"

class UIManager :public Manager<UIManager>
{
	friend class Manager<UIManager>;

private:
	StatusUI* statusUI;              //״̬��UI���ڵ�ǰ��������ʼ��ʱʵ����

public:
	void OnUpdate(SDL_Renderer*);
	void OnRender(SDL_Renderer*);

private:
	UIManager();
	~UIManager();
};

#endif
