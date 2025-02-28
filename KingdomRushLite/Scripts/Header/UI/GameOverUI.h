#ifndef _GAME_OVER_UI_
#define _GAME_OVER_UI_

#include <SDL.h>

class GameOverUI
{
private:
	#pragma region Setting
	#pragma region Size
	SDL_Point winLoseTextSize = { 0, 0 };
	SDL_Point waveNumTextSize = { 0, 0 };
	#pragma endregion

	#pragma region Distance
	int textRowDistance = 4;
	#pragma endregion

	#pragma region Color
	const SDL_Color backgroundColor = { 0,0,0,200 };
	const SDL_Color winLoseTextColor = { 255,0,0,255 };
	const SDL_Color waveNumTextColor = { 255,0,0,255 };
	#pragma endregion
	#pragma endregion

	SDL_Texture* winLoseTextTexture = nullptr;
	SDL_Texture* waveNumTextTexture = nullptr;

public:
	GameOverUI() = default;
	~GameOverUI() = default;

	void OnUpdate(SDL_Renderer*);
	void OnRender(SDL_Renderer*);
};

#endif
