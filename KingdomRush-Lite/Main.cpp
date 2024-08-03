/*注意事项
* 1、此处我们选择运行效率较好的Release模式并将信息打印到控制台进行调试，这种方式也多适用于小型游戏开发
* 2、属性配置中"C/C++"下"代码生成"下的"运行库"配置应当为"多线程(/MT)"，以确保静态链接运行时库
*/

#pragma region Preprocessing
//将此宏定义，否则main函数在预处理阶段会被define为SDL_main
#define SDL_MAIN_HANDLED

#include <SDL.h>
//图像支持
#include <SDL_image.h>
//混音支持
#include <SDL_mixer.h>
//字体支持
#include <SDL_ttf.h>
//图形支持
#include <SDL2_gfxPrimitives.h>
#pragma endregion

int main()
{
	#pragma region Initialize
	//初始化SDL库的所有子系统
	SDL_Init(SDL_INIT_EVERYTHING);
	//初始化SDL_image库的各支持格式
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	//初始化SDL_ttf库
	TTF_Init();
	//初始化SDL_mixer库的支持格式
	Mix_Init(MIX_INIT_MP3);
	//打开混音器的声道，Mix_OpenAudio(音频采样率, 解码音频格式, 声道数, 音频解码缓冲区大小)
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	#pragma endregion
	
	#pragma region Window&Render
	//从屏幕中心显示一个标题为"WhythZ"的1280x720的一般样式的窗口 
	SDL_Window* window = SDL_CreateWindow(u8"WhythZ", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
	//加载渲染器（使用GPU进行加速）到刚刚创建的窗口上
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	#pragma endregion

	//存储鼠标指针位置
	SDL_Point cursorPos = { 0,0 };

	//决定游戏主循环是否结束
	bool isQuit = false;
	//用于调用SDL事件，根据不同的事件类型做出相应的反应
	SDL_Event event;

	//游戏主循环
	while (!isQuit)
	{
		/*处理事件以保证窗口正常交互*/
		while (SDL_PollEvent(&event))
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

		/*渲染绘图*/
		//确定渲染的颜色为纯黑（不透明），接收RGB三色与Alpha（记录图像的透明度信息的256级灰度）
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		//使用设定的不透明黑色填充整个窗口达到清屏的效果
		SDL_RenderClear(renderer);
		//在鼠标指针处绘制一个红色的圆，第四参数位为半径，再往后为RGB和Alpha
		filledCircleRGBA(renderer, cursorPos.x, cursorPos.y, 55, 255, 0, 0, 200);
		//将渲染的内容更新到窗口缓冲区上
		SDL_RenderPresent(renderer);
	}
}