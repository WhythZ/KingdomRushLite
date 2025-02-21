#include "../../../Header/Manager/Concrete/UIManager.h"

UIManager::UIManager()
{
	statusUI = new StatusUI();
}

UIManager::~UIManager()
{
	delete statusUI;
}

void UIManager::OnUpdate(SDL_Renderer* _renderer)
{
	statusUI->OnUpdate(_renderer);
}

void UIManager::OnRender(SDL_Renderer* _renderer)
{
	statusUI->OnRender(_renderer);
}