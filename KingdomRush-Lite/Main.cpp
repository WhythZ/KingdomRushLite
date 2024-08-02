/*注意事项
* 1、此处我们选择运行效率较好的Release模式并将信息打印到控制台进行调试，这种方式也多适用于小型游戏开发
* 2、属性配置中"C/C++"下"代码生成"下的"运行库"配置应当为"多线程(/MT)"，以确保静态链接运行时库
*/

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

int main()
{
	std::cout << "hi" << std::endl;
}