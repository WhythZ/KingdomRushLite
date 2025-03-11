//一、此项目我们选择运行效率较好的Release模式并将信息打印到控制台进行调试，这种方式多适用于小型游戏开发
//二、属性配置中"C/C++"下"代码生成"下的"运行库"配置应当为"多线程(/MT)"，以确保静态链接运行时库

//游戏主管理器
#include "../Header/Manager/Concrete/GameManager.h"

//将此宏定义，否则main函数在预处理阶段会被define为SDL_main
#define SDL_MAIN_HANDLED

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

int main(int _argc, char** _argv)
{
    //在Emscripten平台下，使用emscripten_set_main_loop来替代传统的游戏主循环
    #ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(
		[]()
		{
			GameManager::Instance()->Run(0, nullptr);
		}, 0, 1
	); //0代表没有循环次数，1代表单次执行
	#else
    //非Emscripten环境下直接运行
    return GameManager::Instance()->Run(_argc, _argv);
    #endif
}