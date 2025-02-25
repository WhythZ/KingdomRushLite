#include "../../../Header/Infra/StateMachine/State.h"

State::State(StateMachine* _stateMachine, Animation* _animation)
{
	stateMachine = _stateMachine;
	animation = _animation;
}

void State::OnUpdate(double _delta)
{
	animation->OnUpdate(_delta);
}

void State::OnRender(SDL_Renderer* _renderer)
{
	static SDL_Point _point;
	_point.x = (int)(stateMachine->position->x - stateMachine->size->x / 2);
	_point.y = (int)(stateMachine->position->y - stateMachine->size->y / 2);
	animation->OnRender(_renderer, _point);
}