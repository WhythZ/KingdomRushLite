#ifndef _TIMER_HPP_
#define _TIMER_HPP_

//֧�ֻص�������Callback�����������������ݵĺ�������ͷ�ļ�
#include <functional>

class Timer
{
private:
	double passTime = 0;                            //�Ӽ�ʱ���������󣬹�ȥ�˶೤ʱ��
	double waitTime = 0;                            //�ȴ��೤ʱ��󴥷�ָ������

	bool isPaused = false;                          //��ʱ������ͣ����ͣ�ڼ�passTime���ۻ�ʱ��
	bool isShotted = false;                         //ֻҪ��ʱ������ʱ����һ�Σ���ֵ�ͱ�Ϊ��
	bool isOneShot = false;                         //true��ʾ�ü�ʱ�����ܴ���һ��
	std::function<void()> trigger;                  //��ʱ����ʱ����������ʱ������ʲô�ص�����

public:
	Timer() = default;
	~Timer() = default;

	void SetWaitTime(double);                       //���õȴ�ʱ��
	void SetOneShot(bool);                          //�����Ƿ���Զ�δ���
	void SetTimeOutTrigger(std::function<void()>);  //����<void()>���޲��޷��Ļص�����

	void OnUpdate(double);                          //���¼�ʱ
	void Pause();                                   //��ͣ��ʱ
	void Resume();                                  //������ʱ
	void Restart();                                 //��ȫ���ü�ʱ״̬
};

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
		if (_canShot && trigger) trigger();

		//���ѵ�һ���ȴ�ʱ�����ڣ��Ա���һ�δ���
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

#endif