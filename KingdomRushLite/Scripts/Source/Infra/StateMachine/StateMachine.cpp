#include "../../../Header/Infra/StateMachine/StateMachine.h"

void StateMachine::OnUpdate(double _delta)
{
	currentState->OnUpdate(_delta);
}

void StateMachine::OnRender(SDL_Renderer* _renderer)
{
	currentState->OnRender(_renderer);
}

void StateMachine::ChangeState(State* _state)
{
	//调用旧状态的结束函数
	if (currentState != nullptr)
		currentState->OnEnd();
	
	//赋予新状态，并调用新状态的开始函数
	//formerState = currentState;
	currentState = _state;
	currentState->OnBegin();
}

State* StateMachine::GetCurrentState() const
{
	return currentState;
}