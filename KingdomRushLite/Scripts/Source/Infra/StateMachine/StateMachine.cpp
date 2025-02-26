#include "../../../Header/Infra/StateMachine/StateMachine.h"

void StateMachine::OnUpdate(double _delta)
{
	if (currentState != nullptr)
		currentState->OnUpdate(_delta);
}

void StateMachine::OnRender(SDL_Renderer* _renderer)
{
	if (currentState != nullptr)
		currentState->OnRender(_renderer);
}

void StateMachine::ChangeState(State* _state)
{
	//���þ�״̬�Ľ�������
	if (currentState != nullptr)
		currentState->OnEnd();
	
	//������״̬����������״̬�Ŀ�ʼ����
	//formerState = currentState;
	currentState = _state;
	currentState->OnBegin();
}

State* StateMachine::GetCurrentState() const
{
	return currentState;
}