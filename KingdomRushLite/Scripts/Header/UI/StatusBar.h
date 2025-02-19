#ifndef _STATUS_BAR_H_
#define _STATUS_BAR_H_

#include <SDL.h>

class StatusBar
{
private:
	SDL_Point position = { 0 };

public:
	StatusBar() = default;
	~StatusBar() = default;
	void SetPosition(SDL_Point);

	void OnUpdate(double);
	void OnRender(SDL_Renderer*);
};

#endif
