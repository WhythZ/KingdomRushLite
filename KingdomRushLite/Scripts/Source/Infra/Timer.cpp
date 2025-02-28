#include "../../Header/Infra/Timer.h"

void Timer::SetWaitTime(double _wait)
{
	waitTime = _wait;
}

void Timer::SetOneShot(bool _flag)
{
	isOneShot = _flag;
}

void Timer::SetTimeOutTrigger(std::function<void()> _callback)
{
	trigger = _callback;
}

void Timer::OnUpdate(double _delta)
{
	//����Ƿ�Ӧ����ͣ
	if (isPaused) return;

	//�Դ���ʱ���ۻ�ʱ��
	passTime += _delta;

	//���ﵽ�˵ȴ�ʱ�䣬��ʱ���ͱ�������
	if (passTime >= waitTime)
	{
		//��ֻ�ܴ���һ����û���������������ܱ���δ�������ü�ʱ���ſ��Ա�����
		bool _canShot = (!isOneShot || (isOneShot && !isShotted));

		//����ɴ������һص��������ڣ��򴥷��ص�����
		if (_canShot && trigger)
			trigger();

		//����ɶ�δ����������ѵ�һ���ȴ�ʱ�����ڣ��Ա���һ�δ���
		if (!isOneShot)
			passTime -= waitTime;
	}
}

void Timer::Pause()
{
	isPaused = true;
}

void Timer::Resume()
{
	isPaused = false;
}

void Timer::Restart()
{
	//�����ѵȴ�ʱ��
	passTime = 0;
	//�����ѱ���������״̬
	isShotted = false;
}

double Timer::GetPassTime() const
{
	return passTime;
}