#ifndef _UI_MANAGER_H_
#define _UI_MANAGER_H_

#include "../Manager.hpp"
#include <SDL.h>
#include "../../UI/StatusUI.h"

class UIManager :public Manager<UIManager>
{
	friend class Manager<UIManager>;

private:
	StatusUI* statusUI;              //状态栏UI，在当前管理器初始化时实例化

public:
	void OnUpdate(SDL_Renderer*);
	void OnRender(SDL_Renderer*);

private:
	UIManager();
	~UIManager();
};

#endif
