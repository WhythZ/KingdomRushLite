#include "../../../Header/Manager/Concrete/GameManager.h"
//字体、图像、混音支持
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
//用到的其它管理器类
#include "../../../Header/Manager/Concrete/ProcessManager.h"
#include "../../../Header/Manager/Concrete/ConfigManager.h"
#include "../../../Header/Manager/Concrete/ResourceManager.h"
#include "../../../Header/Manager/Concrete/WaveManager.h"
#include "../../../Header/Manager/Concrete/AudioManager.h"
#include "../../../Header/Manager/Concrete/EnemyManager.h"
#include "../../../Header/Manager/Concrete/BulletManager.h"
#include "../../../Header/Manager/Concrete/TowerManager.h"
#include "../../../Header/Manager/Concrete/DropManager.h"
#include "../../../Header/Manager/Concrete/UIManager.h"
#include "../../../Header/Manager/Concrete/PlayerManager.h"

GameManager::GameManager()
{
	#pragma region SDL
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

	#pragma region LoadData
	//使用初始化断言，加载主配置文件（其内包含窗口配置信息，故而放在窗口初始化之前）、关卡文件、地图文件
	ConfigManager* _config = ConfigManager::Instance();
	InitAssert(_config->LoadMap("Data/Map.csv"), u8"Failed To Load Map.csv");
	InitAssert(_config->LoadConfigs("Data/Configs.json"), u8"Failed To Load Configs.json");
	InitAssert(_config->LoadWaves("Data/Waves.json"), u8"Failed To Load Waves.json");
	//InitAssert(_config->LoadWaves("Data/WavesTest.json"), u8"Failed To Load WavesTest.json");
	#pragma endregion

	#pragma region Window&Renderer
	//从屏幕中心显示一个带标题的特定尺寸的一般样式的窗口
	window = SDL_CreateWindow(_config->basicPrefab.windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		_config->basicPrefab.windowWidth, _config->basicPrefab.windowHeight, SDL_WINDOW_SHOWN);
	//检测窗口是否初始化成功
	InitAssert(window, u8"Failed To Create Window");

	//加载渲染器到窗口window上，第三参数位使用了三种渲染技术
	//SDL_RENDERER_ACCELERATED：硬件加速
	//SDL_RENDERER_PRESENTVSYNC：垂直同步，是一种为了消除屏幕撕裂（产生于窗口视频帧率与显示器刷新率不同步）的显示技术
	//SDL_RENDERER_TARGETTEXTURE：将渲染目标设置为纹理，即先渲染纹理图片，再将图片渲染到窗口，用于瓦片地图的生成
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
	//检测渲染器是否初始化成功
	InitAssert(renderer, u8"Failed To Create Renderer");

	//使得在打开SDL窗口时若是输入中文可以显示候选词列表，以提醒玩家切换为英文输入法，因为中文输入无法被正常捕获
	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
	#pragma endregion

	#pragma region LoadResource
	//加载资源
	InitAssert(ResourceManager::Instance()->LoadResource(renderer), u8"Failed To Load Resources");
	//获取资源后，根据TileSet生成瓦片地图纹理
	InitAssert(GenerateTileMapTexture(), u8"Failed To Genrate TileMap Texture");
	#pragma endregion

	//实例化玩家，暂时只有龙类型
	PlayerManager::Instance()->InstantiatePlayer(PlayerType::Dragon);

	//播放背景音乐
	AudioManager::Instance()->PlayBGM(MusicResID::BGM_InLevel_0);
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
			OnInput();

		//数据更新检测
		OnUpdate(_delta);

		#pragma region Render
		//确定渲染的颜色为纯黑（不透明），接收RGB三色与Alpha（记录图像的透明度信息的256级灰度）
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		//使用设定的不透明黑色填充整个窗口达到清屏的效果
		SDL_RenderClear(renderer);

		//在经历了上述准备后，进行具体的渲染绘图
		OnRender();
		//将渲染的内容更新到窗口缓冲区上
		SDL_RenderPresent(renderer);
		#pragma endregion
	}

	return 0;
}

SDL_Point GameManager::GetCursorTileIdx() const
{
	static SDL_Point _pt;
	_pt.x = (cursorPosition.x % TILE_SIZE == 0) ? cursorPosition.x / TILE_SIZE - 1 : cursorPosition.x / TILE_SIZE;
	_pt.y = (cursorPosition.y % TILE_SIZE == 0) ? cursorPosition.y / TILE_SIZE - 1 : cursorPosition.y / TILE_SIZE;
	//std::cout << "CursorPosition=(" << cursorPosition.x << "," << cursorPosition.y << "),CursorTileIdx=(" << _pt.x << "," << _pt.y << ")\n";
	return _pt;
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

void GameManager::OnInput()
{
	//游戏没结束时，才会进行下列游戏内玩法相关管理器的输入检测
	if (!ProcessManager::Instance()->isGameOver)
	{
		UIManager::Instance()->OnInput(event);
		PlayerManager::Instance()->OnInput(event);
	}

	//点击窗口的退出键时触发的SDL_QUIT事件
	if (event.type == SDL_QUIT)
		isQuit = true;
	//鼠标指针移动事件
	if (event.type == SDL_MOUSEMOTION)
	{
		//获取鼠标指针坐标
		cursorPosition.x = event.motion.x;
		cursorPosition.y = event.motion.y;
	}
}

void GameManager::OnUpdate(double _delta)
{
	//游戏没结束时，才会进行下列管理器的更新
	if (!ProcessManager::Instance()->isGameOver)
	{
		WaveManager::Instance()->OnUpdate(_delta);
		EnemyManager::Instance()->OnUpdate(_delta);
		BulletManager::Instance()->OnUpdate(_delta);
		TowerManager::Instance()->OnUpdate(_delta);
		DropManager::Instance()->OnUpdate(_delta);
		UIManager::Instance()->OnUpdate(renderer);
		PlayerManager::Instance()->OnUpdate(_delta);
	}
}

void GameManager::OnRender()
{
	#pragma region GameWindow
	//用于从目标窗口中切割出一块区域，用于塞入mapTexture的画面内容
	SDL_Rect _dst = ProcessManager::Instance()->mapRect;
	//将mapTexture渲染在_dst从窗口中切割出的区域内
	SDL_RenderCopy(renderer, mapTexture, nullptr, &_dst);
	#pragma endregion

	//渲染敌人、子弹、防御塔、掉落物、UI
	EnemyManager::Instance()->OnRender(renderer);
	BulletManager::Instance()->OnRender(renderer);
	TowerManager::Instance()->OnRender(renderer);
	DropManager::Instance()->OnRender(renderer);
	PlayerManager::Instance()->OnRender(renderer);
	//注意函数调用的顺序，UI应当是最后渲染的，以保证始终在最上层
	UIManager::Instance()->OnRender(renderer);
}

bool GameManager::GenerateTileMapTexture()
{
	#pragma region PrepareMapTextureCanvas
	//获取游戏的地图与瓦片地图的常引用
	const Map& _map = ProcessManager::Instance()->map;
	const TileMap& _tileMap = _map.GetTileMap();

	//最终渲染出的瓦片地图的纹理宽高
	int _tileMapWidth = (int)_map.GetWidthTileNum() * TILE_SIZE;
	int _tileMapHeight = (int)_map.GetHeightTileNum() * TILE_SIZE;

	//对最终要被渲染在屏幕上的地图纹理图片进行编辑
	//第一参数即渲染器（即作为画笔在纹理画布上画画），第四五位参数即渲染画布的宽高
	//纹理排布格式是SDL_PIXELFORMAT_ARGB8888（8位即0~255范围的Alpha的RGB），访问权限是SDL_TEXTUREACCESS_TARGET（即纹理可以作为画布被渲染器渲染）
	mapTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, _tileMapWidth, _tileMapHeight);
	//渲染失败（比如图片太大，显存不够）则返回false
	if (!mapTexture) return false;
	#pragma endregion

	#pragma region PrepareMapRect
	//直接修改mapRect，在此函数中仅做写操作，其读操作在函数OnRender()中进行，用于指定切割mapTexture在窗口中的渲染位置
	SDL_Rect& _mapRect = ProcessManager::Instance()->mapRect;

	//更改地图纹理的渲染位置（SDL_Rect对象：成员x和y表示纹理图片的矩形左上角顶点的坐标、成员w和h表示矩形的宽高）
	//利用定义好的（固定的）窗口宽度减去嵌在窗口中间的矩形地图的宽度（参考“回”字的结构）后再除以2即可得到地图纹理矩形左上定点的横坐标，纵坐标同理
	_mapRect.x = (ConfigManager::Instance()->basicPrefab.windowWidth - _tileMapWidth) / 2;
	_mapRect.y = (ConfigManager::Instance()->basicPrefab.windowHeight - _tileMapHeight) / 2;
	//宽高的值是绝对的（x和y坐标表示的点的位置是相对于窗口的），所以直接赋值即可
	_mapRect.w = _tileMapWidth;
	_mapRect.h = _tileMapHeight;
	#pragma endregion

	//设置地图纹理的混合渲染模式（SDL_BLENDMODE_BLEND即启用透明度的渲染）
	SDL_SetTextureBlendMode(mapTexture, SDL_BLENDMODE_BLEND);
	//将渲染器renderer的渲染目标指定到mapTexture上（在使用完成后一定要记得置空目标到nullptr上）
	SDL_SetRenderTarget(renderer, mapTexture);

	#pragma region PrepareSegmentTileSetTexture
	//获取TileSet的纹理图片准备对其进行切割取材
	//不能用ResourceManager::GetInstance()->GetSpritePool()[SpriteResID::Tile_TileSet]获取池内的资源对象
	//因为若池内没有目标ID的话，unordered_map就会创建一个，而GetSpritePool()返回的是const的资源池，是不允许上述可能产生的更改的
	//而函数find(XXX)返回的就是以XXX为键的键值对，->second指的是取出该键值对的值
	SDL_Texture* _tileSetTexture = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Tile_TileSet)->second;

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

	//逐瓦片对mapTexture进行渲染
	for (int y = 0; y < _map.GetHeightTileNum(); y++)
	{
		for (int x = 0; x < _map.GetWidthTileNum(); x++)
		{
			//存储TileSet中的对应瓦片素材的矩形纹理图片的位置（矩形左上角顶点坐标xy+矩形宽高wh）
			SDL_Rect _src;
			//被遍历的瓦片地图中的瓦片，其指定了应当渲染TileSet中的特定素材纹理
			const Tile& _tile = _tileMap[y][x];

			//_src将被渲染到mapTexture中的_dst位置上
			const SDL_Rect& _dst =
			{
				//目标瓦片矩形左上角顶点的坐标
				x * TILE_SIZE, y * TILE_SIZE,
				//瓦片是边长固定的正方形
				TILE_SIZE, TILE_SIZE
			};

			#pragma region RenderTerrainLayer
			//根据_tile内存储的信息，决定应当如何裁取TileSet以获取正确的图片
			//先从地形层（每个非负数索引一个瓦片）开始，因为其应当被放在最底层（最后被渲染在同一区块的纹理会显示在顶层）
			_src =
			{
				//terrainLayer是地形层指定的渲染瓦片索引，设想TileSet一行有5列瓦片，若terrainLayer等于11（索引0表示第一个瓦片），
				//那么就要取第3行的第2列瓦片作为渲染目标，11%5=1表示第2列没毛病，11/5=2表示第3行没毛病
				(_tile.terrainLayer % _rowTileNum) * TILE_SIZE,
				(_tile.terrainLayer / _rowTileNum) * TILE_SIZE,
				//宽高固定不变，没啥好说的
				TILE_SIZE, TILE_SIZE
			};
			//将从纹理块_src拷贝渲染到目标区块_dst上
			//第二参数位的参数类型是SDL_Texture*，第三参数位是SDL_Rect*，即对前者的裁剪
			//第四参数位的SDL_Rect*对象则是对mapTexture（也是SDL_Texture*）的裁剪
			SDL_RenderCopy(renderer, _tileSetTexture, &_src, &_dst);
			#pragma endregion

			#pragma region RenderDecorationLayer
			//渲染装饰层（每个非负数索引一个瓦片）
			if (_tile.decorationLayer > 0)
			{
				_src =
				{
					//这里存在一个潜在问题，即地形层和装饰层可能会由于设置不当而重复渲染同一个索引的瓦片
					(_tile.decorationLayer % _rowTileNum) * TILE_SIZE,
					(_tile.decorationLayer / _rowTileNum) * TILE_SIZE,
					TILE_SIZE, TILE_SIZE
				};
				//此处的渲染会覆盖在地形层之上
				SDL_RenderCopy(renderer, _tileSetTexture, &_src, &_dst);
			}
			#pragma endregion
		}
	}

	#pragma region RenderHome
	//获取家在瓦片地图中的位置索引（瓦片地图中应当渲染家的纹理的位置瓦片矩形的左上顶点的坐标）
	const SDL_Point& _homeIdx = _map.GetHomeIdx();
	//通过索引锁定对应的纹理区域（SDL_Point的xy即是SDL_Rect的xy）
	const SDL_Rect& _homeDst =
	{
		//家的坐标在瓦片地图中对应的SDL_Rect纹理图片的左上顶点位置
		_homeIdx.x * TILE_SIZE, _homeIdx.y * TILE_SIZE,
		//瓦片是边长固定的正方形
		TILE_SIZE, TILE_SIZE
	};
	//如果家的瓦片纹理在TileSet中，就需要从中寻找，但我们已经将其提取出来放到资源池里了，就可以直接用
	SDL_Texture* _homeSrc = ResourceManager::Instance()->GetTexturePool().find(TextureResID::Tile_Home)->second;
	SDL_RenderCopy(renderer, _homeSrc, nullptr, &_homeDst);
	#pragma endregion

	//置空渲染目标（即渲染窗口）
	SDL_SetRenderTarget(renderer, nullptr);

	return true;
}
