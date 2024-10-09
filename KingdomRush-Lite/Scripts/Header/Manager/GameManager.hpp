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
//基类与其它管理器类
#include "Manager.hpp"
#include "ConfigManager.hpp"
#include "ResourceManager.hpp"

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
	SDL_Texture* mapTexture = nullptr;   //存储地图渲染的纹理图片

public:
	int Run(int, char**);                //将游戏主循环封装在此函数内（为了使main函数主体保持整洁）

private:
	GameManager();                       //构造了什么就要释放什么，以防止内存泄漏与减少内存占用
	~GameManager();                      //析构的顺序应当与构造的顺序相反，因为构造的顺序暗含依赖的关系，被依赖项不该被先释放
	
	void InitAssert(bool, const char*);  //初始化断言，用于初始化，并依据初始化函数返回的参数判断是否初始化成功
	void On_Input();                     //主循环内检测拉取的输入事件，并对应地作出反应
	void On_UpdateData(double);          //主循环内检测数据的更新
	void On_Render();                    //主循环内渲染绘图的具体过程
	bool GenerateTileMapTileMap();       //将所有瓦片生成整张瓦片地图纹理，以提升渲染性能
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
	#pragma region SDL-Libraries
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
	#pragma endregion

	#pragma region LoadConfig
	//加载配置文件（文件会被加载到这个指针指向的单例管理器上）
	ConfigManager* _config = ConfigManager::GetInstance();
	//使用初始化断言，加载地图文件、主配置文件（其内包含窗口配置信息，故而放在窗口初始化之前）、关卡文件
	InitAssert(_config->map.Load("Data/map.csv"), u8"Failed To Load map.csv");
	InitAssert(_config->LoadConfig("Data/config.json"), u8"Failed To Load config.json");
	InitAssert(_config->LoadLevel("Data/level.json"), u8"Failed To Load level.json");
	#pragma endregion

	#pragma region Window&Renderer
	//从屏幕中心显示一个标题为"KingdomRush-Lite"的1280x720的一般样式的窗口
	window = SDL_CreateWindow(_config->basicPrefab.windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		_config->basicPrefab.windowWidth, _config->basicPrefab.windowHeight, SDL_WINDOW_SHOWN);
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
	#pragma endregion

	//加载资源
	InitAssert(ResourceManager::GetInstance()->LoadResource(renderer), u8"Failed To Load Resources");

	//生成瓦片地图纹理
	InitAssert(GenerateTileMapTileMap(), u8"Failed To Genrate TileMap Texture");
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
	if (_flag) return;
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

bool GameManager::GenerateTileMapTileMap()
{
	#pragma region Preparation
	//获取配置管理器的单例
	ConfigManager* _config = ConfigManager::GetInstance();
	//获取游戏的地图与瓦片地图的常引用
	const Map& _map = _config->map;
	const TileMap& _tileMap = _map.GetTileMap();
	//准备修改地图在游戏窗口内部渲染地图纹理的位置，即存储的是地图矩形的左上角定点坐标
	SDL_Rect& _mapRect = _config->mapRect;
	#pragma endregion

	#pragma region SegmentTileSetTex
	//获取TileSet的纹理图片准备对其进行切割取材
	//不能用ResourceManager::GetInstance()->GetSpritePool()[SpriteResID::Tile_TileSet]获取池内的资源对象
	//因为若池内没有目标ID的话，unordered_map就会创建一个，而GetSpritePool()返回的是const的资源池，是不允许上述可能产生的更改的
	//而函数find(XXX)返回的就是以XXX为键的键值对，->second指的是取出该键值对的值
	SDL_Texture* _tileSetTexture = ResourceManager::GetInstance()->GetSpritePool().find(SpriteResID::Tile_TileSet)->second;

	//存储TileSet的宽高
	int _tileSetWidth, _tileSetHeight;
	//查询纹理属性，第一个参数传入（被访问的）纹理的指针，第二三参数位传入的是纹理格式与访问权限（用不到就置空即可）
	//第四五传入宽高的地址，被访问的纹理对象的宽高信息就会被存储到对应的传入地址上
	SDL_QueryTexture(_tileSetTexture, nullptr, nullptr, &_tileSetWidth, &_tileSetHeight);
	
	//计算该TileSet的一行可以提取几个瓦片（这与图片的宽度、宏TILE_SIZE的大小有关，此处并不是最通用的图片切割算法，后续可扩展）
	int _rowTileNum = (int)std::ceil((double)(_tileSetWidth / TILE_SIZE));
	//计算该TileSet的一列可以提取几个瓦片
	int _columnTileNum = (int)std::ceil((double)(_tileSetHeight / TILE_SIZE));
	#pragma endregion

	#pragma region PrepareTileMapTexCanvas
	//最终渲染出的瓦片地图的纹理宽高
	int _tileMapWidth = (int)_map.GetWidthTileNum() * TILE_SIZE;
	int _tileMapHeight = (int)_map.GetHeightTileNum() * TILE_SIZE;

	//对最终要被渲染在屏幕上的地图纹理图片进行编辑
	//第一参数即渲染器（即作为画笔在纹理画布上画画），第四五位参数即渲染画布的宽高
	//纹理排布格式是SDL_PIXELFORMAT_ARGB8888（8位即0~255范围的Alpha的RGB），访问权限是SDL_TEXTUREACCESS_TARGET（即纹理可以作为画布被渲染器渲染）
	mapTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, _tileMapWidth, _tileMapHeight);
	//渲染失败（比如图片太大，显存不够）则返回false
	if (!mapTexture) return false;

	//更改配置文件中的地图纹理的渲染位置（SDL_Rect对象：成员x和y表示纹理图片的矩形左上角顶点的坐标、成员w和h表示矩形的宽高）
	//利用定义好的（固定的）窗口宽度减去嵌在窗口中间的矩形地图的宽度（参考“回”字的结构）后再除以2即可得到地图纹理矩形左上定点的横坐标，纵坐标同理
	_mapRect.x = (_config->basicPrefab.windowWidth - _tileMapWidth) / 2;
	_mapRect.y = (_config->basicPrefab.windowHeight - _tileMapHeight) / 2;
	//宽高的值是绝对的（x和y坐标表示的点的位置是相对于窗口的），所以直接赋值即可
	_mapRect.w = _tileMapWidth;
	_mapRect.h = _tileMapHeight;
	#pragma endregion

	//设置地图纹理的混合渲染模式（SDL_BLENDMODE_BLEND即启用透明度的渲染）
	SDL_SetTextureBlendMode(mapTexture, SDL_BLENDMODE_BLEND);
	//将渲染器renderer的渲染目标指定到mapTexture上（在使用完成后一定要记得置空目标到nullptr上）
	SDL_SetRenderTarget(renderer, mapTexture);

	//逐瓦片对mapTexture进行渲染
	for (int y = 0; y < _map.GetWidthTileNum(); y++)
	{
		for (int x = 0; x < _map.GetHeightTileNum(); x++)
		{

		}
	}

	//置空渲染目标（即渲染窗口）
	SDL_SetRenderTarget(renderer, nullptr);

	return true;
}

#endif
