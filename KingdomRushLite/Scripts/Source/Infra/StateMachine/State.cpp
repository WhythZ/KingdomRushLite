#include "../../../Header/Infra/StateMachine/State.h"

State::State(bool* _flag)
{
	flag = _flag;
}

void State::OnBegin()
{
	*flag = true;

	//ÿ�ν��뵱ǰ״̬����Ҫ���¼�ʱ
	stateTimer.Restart();
}

void State::OnUpdate(double _delta)
{
	//���¼�ʱ�����У��������������ʼ������Ϊ�ü�ʱ�������ڼ�¼���뵱ǰ״̬��ʱ��
	stateTimer.OnUpdate(_delta);
}

void State::OnEnd()
{
	*flag = false;
}