#include "../../../Header/Infra/StateMachine/State.h"

State::State(bool* _flag)
{
	flag = _flag;
}

void State::OnBegin()
{
	*flag = true;

	//每次进入当前状态都需要重新计时
	stateTimer.Restart();
}

void State::OnUpdate(double _delta)
{
	//更新计时器就行，无需进行其它初始化，因为该计时器仅用于记录进入当前状态的时间
	stateTimer.OnUpdate(_delta);
}

void State::OnEnd()
{
	*flag = false;
}