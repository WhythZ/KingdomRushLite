#ifndef _GAME_MANAGER_HPP_
#define _GAME_MANAGER_HPP_

//引入SDL库
#include <SDL.h>
//字体支持
#include <SDL_ttf.h>
//图像支持
#include <SDL_image.h>
//混音支持
#include <SDL_mixer.h>
//用于继承基类
#include "Manager.hpp"

//游戏主管理器
class GameManager :public Manager<GameManager>
{
	//授予Manager基类的GetInstance函数权限以调用GameManager类的构造函数
	friend class Manager<GameManager>;

private:
	SDL_Window* window = nullptr;        //存储游戏主窗口，用于显示游戏画面
	SDL_Renderer* renderer = nullptr;    //存储渲染器，渲染器可以使用GPU进行加速
	bool isQuit = false;                 //决定游戏主循环是否结束
	int fps = 60;                        //需要维持的游戏帧率，即每秒刷新的帧数
	SDL_Event event;                     //用于调用SDL事件，根据不同的事件类型做出相应的反应
	SDL_Point cursorPos = { 0,0 };       //存储鼠标指针位置，其位置在事件检测更新中被刷新

public:
	int Run(int, char**);                //将游戏主循环封装在此函数内（为了使main函数主体保持整洁）

private:
	GameManager();                       //构造了什么就要释放什么，以防止内存泄漏与减少内存占用
	~GameManager();                      //析构的顺序应当与构造的顺序相反，因为构造的顺序暗含依赖的关系，被依赖项不该被先释放
	
	void InitAssert(bool, const char*);  //初始化断言，用于初始化各库，并依据各库的初始化函数返回的参数判断是否初始化成功
	void On_Input();                     //主循环内检测拉取的输入事件，并对应地作出反应
	void On_UpdateData(double);          //主循环内检测数据的更新
	void On_Render();                    //主循环内渲染绘图的具体过程
};

int GameManager::Run(int _argc, char** _argv)
{
	#pragma region LimitFPS
	//此函数获取一个高性能（精度较高）计时器，函数返回的值（计时器跳的总数）作为计时器的起点，通过作差后除以频率才有意义
	Uint64 _lastCounter = SDL_GetPerformanceCounter();
	//频率即每一秒此计时器会跳多少次
	Uint64 _counterFreq = SDL_GetPerformanceFrequency();
	#pragma endregion

	//游戏主循环
	while (!isQuit)
	{
		#pragma region LimitFPS
		//获取当前的计时器跳的总数，用以与主循环前得到的计时器总数作差
		Uint64 _currentCounter = SDL_GetPerformanceCounter();
		//作差后转换为双精度浮点，除以频率得到每次循环的时间间隔（单位为秒）
		double _delta = (double)(_currentCounter - _lastCounter) / _counterFreq;
		//将当前的次数作为起点，进行下一次循环
		_lastCounter = _currentCounter;
		//动态延时控制帧率，若是帧率超过了限定值，那么就将多余的时间延迟掉防止主循环频率过快；乘以1000是将秒转化为毫秒，因为秒这个单位太大而精度不高
		if (_delta * 1000 < 1000.0 / fps)
			SDL_Delay((Uint32)(1000.0 / fps - _delta * 1000));
		#pragma endregion

		//拉取并处理事件以保证窗口正常交互
		while (SDL_PollEvent(&event))
			On_Input();

		//数据更新检测
		On_UpdateData(_delta);

		#pragma region Render
		//确定渲染的颜色为纯黑（不透明），接收RGB三色与Alpha（记录图像的透明度信息的256级灰度）
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		//使用设定的不透明黑色填充整个窗口达到清屏的效果
		SDL_RenderClear(renderer);
		//在经历了上述准备后，进行具体的渲染绘图
		On_Render();
		//将渲染的内容更新到窗口缓冲区上
		SDL_RenderPresent(renderer);
		#pragma endregion
	}

	return 0;
}

GameManager::GameManager()
{
	//初始化SDL库的所有子系统；因为SDL_Init函数返回0表示成功，所以此处第一个传入参数取反
	InitAssert(!SDL_Init(SDL_INIT_EVERYTHING), u8"Failed To Init SDL");
	//初始化SDL_ttf库；TTF_Init函数返回0表示成功
	InitAssert(!TTF_Init(), u8"Failed To Init SDL_ttf");
	//初始化SDL_image库的各支持格式；IMG_Init函数返回非零值表示成功，所以直接传入即可
	InitAssert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG), u8"Failed To Init SDL_image");
	//初始化SDL_mixer库的支持格式；Mix_Init函数返回非零值表示成功
	InitAssert(Mix_Init(MIX_INIT_MP3), u8"Failed To Init SDL_mixer");
	//打开混音器的声道，Mix_OpenAudio(音频采样率, 解码音频格式, 声道数, 音频解码缓冲区大小)
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	//从屏幕中心显示一个标题为"KingdomRush-Lite"的1280x720的一般样式的窗口 
	window = SDL_CreateWindow(u8"KingdomRush-Lite", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
	//检测窗口是否初始化成功
	InitAssert(window, "Failed To Create Window");

	//加载渲染器到窗口window上，第三参数位使用了三种渲染技术
	//SDL_RENDERER_ACCELERATED：硬件加速
	//SDL_RENDERER_PRESENTVSYNC：垂直同步，是一种为了消除屏幕撕裂（产生于窗口视频帧率与显示器刷新率不同步）的显示技术
	//SDL_RENDERER_TARGETTEXTURE：将渲染目标设置为纹理，即先渲染纹理图片，再将图片渲染到窗口，用于瓦片地图的生成
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
	//检测渲染器是否初始化成功
	InitAssert(renderer, "Failed To Create Renderer");

	//向SDL提出建议，使得在打开SDL窗口的时候若是输入中文可以显示候选词列表
	//SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
}

GameManager::~GameManager()
{
	//注意析构顺序，先销毁渲染器与窗口
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	//最后释放库的初始化内容，因为上面两个是依赖于库而存在的
	Mix_Quit();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

void GameManager::InitAssert(bool _flag, const char* _errMsg)
{
	//如果初始化成功，那么无事发生，直接返回
	if (_flag)
		return;
	//如果初始化失败，那么就用SDL创建一个报错消息窗口，该函数(消息类型, 窗口标题, 详细报错信息, 作为谁的子窗口)
	//第四个参数如果填NULL，那么报错窗口不作为子窗口；若是有传入窗口，那么报错窗口将作为传入的窗口的子窗口，若子窗口不关掉，父窗口无法被用户交互
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"Game Init Failed", _errMsg, window);
	//以-1结束程序运行
	exit(-1);
}

void GameManager::On_Input()
{
	//点击窗口的退出键时触发的SDL_QUIT事件
	if (event.type == SDL_QUIT)
		isQuit = true;
	//鼠标指针移动事件
	if (event.type == SDL_MOUSEMOTION)
	{
		//获取鼠标指针坐标
		cursorPos.x = event.motion.x;
		cursorPos.y = event.motion.y;
	}
}

void GameManager::On_UpdateData(double delta)
{
}

void GameManager::On_Render()
{
}

#endif
