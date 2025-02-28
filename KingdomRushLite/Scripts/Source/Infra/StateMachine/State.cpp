#include "../../../Header/Infra/StateMachine/State.h"

State::State(bool* _flag)
{
	flag = _flag;
}

void State::OnBegin()
{
	*flag = true;
}

void State::OnEnd()
{
	*flag = false;
}