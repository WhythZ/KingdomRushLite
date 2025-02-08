#ifndef _TIMER_HPP_
#define _TIMER_HPP_

//支持回调函数（Callback，即被当作参数传递的函数）的头文件
#include <functional>

class Timer
{
private:
	double passTime = 0;                            //从计时器被创建后，过去了多长时间
	double waitTime = 0;                            //等待多长时间后触发指定功能

	bool isPaused = false;                          //计时器的暂停，暂停期间passTime不累积时间
	bool isShotted = false;                         //只要计时器被到时触发一次，该值就变为真
	bool isOneShot = false;                         //true表示该计时器仅能触发一次
	std::function<void()> trigger;                  //计时器计时结束被触发时，触发什么回调函数

public:
	Timer() = default;
	~Timer() = default;

	void SetWaitTime(double);                       //设置等待时间
	void SetOneShot(bool);                          //设置是否可以多次触发
	void SetTimeOutTrigger(std::function<void()>);  //设置<void()>即无参无返的回调函数

	void OnUpdate(double);                          //更新计时
	void Pause();                                   //暂停计时
	void Resume();                                  //继续计时
	void Restart();                                 //完全重置计时状态
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
	//检测是否应当暂停
	if (isPaused) return;

	//以传入时间累积时间
	passTime += _delta;
	
	//若达到了等待时间，计时器就被触发了
	if (passTime >= waitTime)
	{
		//若只能触发一次且没被触发过，或者能被多次触发，则该计时器才可以被触发
		bool _canShot = (!isOneShot || (isOneShot && !isShotted));

		//如果可触发，且回调函数存在，则触发回调函数
		if (_canShot && trigger) trigger();

		//消费掉一个等待时间周期，以便下一次触发
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
	//归零已等待时间
	passTime = 0;
	//重置已被触发过的状态
	isShotted = false;
}

#endif