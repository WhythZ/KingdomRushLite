//一、此项目我们选择运行效率较好的Release模式并将信息打印到控制台进行调试，这种方式多适用于小型游戏开发
//二、属性配置中"C/C++"下"代码生成"下的"运行库"配置应当为"多线程(/MT)"，以确保静态链接运行时库

//游戏主管理器
#include "../Header/Manager/Concrete/GameManager.h"

//将此宏定义，否则main函数在预处理阶段会被define为SDL_main
#define SDL_MAIN_HANDLED

int main(int _argc, char** _argv)
{
	//游戏主循环
	return GameManager::Instance()->Run(_argc, _argv);
}