#ifndef _GAME_MANAGER_HPP_
#define _GAME_MANAGER_HPP_

//引入SDL库
#include <SDL.h>
#include "../Manager.hpp"

//游戏主管理器
class GameManager :public Manager<GameManager>
{
	//授予Manager基类的GetInstance函数权限以调用GameManager类的构造函数
	friend class Manager<GameManager>;

private:
	SDL_Window* window = nullptr;        //存储游戏主窗口，用于显示游戏画面
	SDL_Renderer* renderer = nullptr;    //存储渲染器，渲染器可以使用GPU进行加速
	SDL_Event event;                     //用于调用SDL事件，根据不同的事件类型做出相应的反应
	bool isQuit = false;                 //决定游戏主循环是否结束
	int fps = 60;                        //需要维持的游戏帧率，即每秒刷新的帧数
	
	SDL_Point cursorPos = { 0,0 };       //存储鼠标指针位置，其位置在事件检测更新中被刷新
	SDL_Texture* mapTexture = nullptr;   //存储被渲染成一整张SDL_Texture*纹理图片的瓦片地图

public:
	int Run(int, char**);                //将游戏主循环封装在此函数内（为了使main函数主体保持整洁）

private:
	GameManager();                       //构造了什么就要释放什么，以防止内存泄漏与减少内存占用
	~GameManager();                      //析构的顺序应当与构造的顺序相反，因为构造的顺序暗含依赖的关系，被依赖项不该被先释放
	void InitAssert(bool, const char*);  //初始化断言，用于初始化，并依据初始化函数返回的参数判断是否初始化成功
	
	void OnInput();                      //主循环内检测输入事件并作出相应的反应
	void OnUpdate(double);               //主循环内逐帧检测数据的更新
	void OnRender();                     //主循环内渲染绘图的具体过程
	
	bool GenerateTileMapTexture();       //将所有瓦片生成整张瓦片地图纹理，以提升渲染性能
};

#endif
