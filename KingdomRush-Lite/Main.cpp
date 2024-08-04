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

//文件读写
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

//用于解析JSON文件
#include <cJSON.h>
#pragma endregion

void Test_JSON()
{
	//加载路径下的文件
	std::ifstream testFile("GameData/test.json");
	//检查是否成功加载
	if (!testFile.good())
	{
		std::cout << "JSON File Note Found" << std::endl;
		return;
	}

	//用于存储被读取文件的数据
	std::stringstream fileData;
	//将所有读取到的内容防到这个容器内
	fileData << testFile.rdbuf();
	//关掉被读取文件
	testFile.close();

	//将字符串数据（即JSON最外圈的花括号及其包含的内容）解析到jsonRoot容器上
	cJSON* jsonRoot = cJSON_Parse(fileData.str().c_str());

	//从jsonRoot中以键名读取对应的值
	cJSON* json_alive = cJSON_GetObjectItem(jsonRoot, "alive");
	cJSON* json_name = cJSON_GetObjectItem(jsonRoot, "name");
	cJSON* json_age = cJSON_GetObjectItem(jsonRoot, "age");
	cJSON* json_hobby = cJSON_GetObjectItem(jsonRoot, "hobby");
	cJSON* json_friend = cJSON_GetObjectItem(jsonRoot, "friend");

	/*展示被成功读取的数据的键值对*/
	//展示读取的布尔值
	std::cout << json_alive->string << " : " << json_alive->valueint << std::endl;
	//展示读取的字符串
	std::cout << json_name->string << " : " << json_name->valuestring << std::endl;
	//展示读取的整型（浮点型对应valuedouble）
	std::cout << json_age->string << " : " << json_age->valueint << std::endl;
	//展示读取的数组
	std::cout << json_hobby->string << " : " << std::endl;
	cJSON* item_ptr = nullptr;
	cJSON_ArrayForEach(item_ptr, json_hobby)
	{
		//上面那个宏接收一个临时的工具空指针和需要遍历的JSON元素对象，在这里执行一个for循环，遍历数组中的元素
		std::cout << "\t" << item_ptr->valuestring << std::endl;
	}
	//不知道怎么解析嵌套JSON，感觉没必要写那么复杂
	//std::cout << json_friend->string << " : " << json_friend->type << std::endl;
}

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

	#pragma region ControlFPS
	//需要维持的游戏帧率（frame per second），代表每秒刷新的帧数
	int fps = 60;
	//此函数获取一个高性能（精度较高）计时器，函数返回的值（计时器跳的总数）作为计时器的起点，通过作差后除以频率才有意义
	Uint64 lastCounter = SDL_GetPerformanceCounter();
	//频率即每一秒此计时器会跳多少次
	Uint64 counterFreq = SDL_GetPerformanceFrequency();
	#pragma endregion

	//决定游戏主循环是否结束
	bool isQuit = false;
	//用于调用SDL事件，根据不同的事件类型做出相应的反应
	SDL_Event event;
	//存储鼠标指针位置
	SDL_Point cursorPos = { 0,0 };

	Test_JSON();

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

		#pragma region ControlFPS
		/*计时器*/
		//获取当前的计时器跳的总数，用以与主循环前得到的计时器总数作差
		Uint64 currentCounter = SDL_GetPerformanceCounter();
		//作差后转换为双精度浮点，除以频率得到每次循环的时间间隔（单位为秒）
		double delta = (double)(currentCounter - lastCounter) / counterFreq;
		//将当前的次数作为起点，进行下一次循环
		lastCounter = currentCounter;
		//若是帧率超过了限定值，那么就将多余的时间延迟掉防止主循环频率过快；乘以1000是将秒转化为毫秒，因为秒这个单位太大而精度不高
		if (delta * 1000 < 1000.0 / fps)
			SDL_Delay((Uint32)(1000.0 / fps - delta * 1000));
		#pragma endregion

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