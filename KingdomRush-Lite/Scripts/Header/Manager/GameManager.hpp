#ifndef _GAME_MANAGER_HPP_
#define _GAME_MANAGER_HPP_

//����SDL��
#include <SDL.h>
//����֧��
#include <SDL_ttf.h>
//ͼ��֧��
#include <SDL_image.h>
//����֧��
#include <SDL_mixer.h>
//���ڼ̳л���
#include "Manager.hpp"

//��Ϸ��������
class GameManager :public Manager<GameManager>
{
	//����Manager�����GetInstance����Ȩ���Ե���GameManager��Ĺ��캯��
	friend class Manager<GameManager>;

private:
	SDL_Window* window = nullptr;        //�洢��Ϸ�����ڣ�������ʾ��Ϸ����
	SDL_Renderer* renderer = nullptr;    //�洢��Ⱦ������Ⱦ������ʹ��GPU���м���
	bool isQuit = false;                 //������Ϸ��ѭ���Ƿ����
	int fps = 60;                        //��Ҫά�ֵ���Ϸ֡�ʣ���ÿ��ˢ�µ�֡��
	SDL_Event event;                     //���ڵ���SDL�¼������ݲ�ͬ���¼�����������Ӧ�ķ�Ӧ
	SDL_Point cursorPos = { 0,0 };       //�洢���ָ��λ�ã���λ�����¼��������б�ˢ��

public:
	int Run(int, char**);                //����Ϸ��ѭ����װ�ڴ˺����ڣ�Ϊ��ʹmain�������屣�����ࣩ

private:
	GameManager();                       //������ʲô��Ҫ�ͷ�ʲô���Է�ֹ�ڴ�й©������ڴ�ռ��
	~GameManager();                      //������˳��Ӧ���빹���˳���෴����Ϊ�����˳�򰵺������Ĺ�ϵ����������ñ����ͷ�
	
	void InitAssert(bool, const char*);  //��ʼ�����ԣ����ڳ�ʼ�����⣬�����ݸ���ĳ�ʼ���������صĲ����ж��Ƿ��ʼ���ɹ�
	void On_Input();                     //��ѭ���ڼ����ȡ�������¼�������Ӧ��������Ӧ
	void On_UpdateData(double);          //��ѭ���ڼ�����ݵĸ���
	void On_Render();                    //��ѭ������Ⱦ��ͼ�ľ������
};

int GameManager::Run(int _argc, char** _argv)
{
	#pragma region LimitFPS
	//�˺�����ȡһ�������ܣ����Ƚϸߣ���ʱ�����������ص�ֵ����ʱ��������������Ϊ��ʱ������㣬ͨ����������Ƶ�ʲ�������
	Uint64 _lastCounter = SDL_GetPerformanceCounter();
	//Ƶ�ʼ�ÿһ��˼�ʱ���������ٴ�
	Uint64 _counterFreq = SDL_GetPerformanceFrequency();
	#pragma endregion

	//��Ϸ��ѭ��
	while (!isQuit)
	{
		#pragma region LimitFPS
		//��ȡ��ǰ�ļ�ʱ��������������������ѭ��ǰ�õ��ļ�ʱ����������
		Uint64 _currentCounter = SDL_GetPerformanceCounter();
		//�����ת��Ϊ˫���ȸ��㣬����Ƶ�ʵõ�ÿ��ѭ����ʱ��������λΪ�룩
		double _delta = (double)(_currentCounter - _lastCounter) / _counterFreq;
		//����ǰ�Ĵ�����Ϊ��㣬������һ��ѭ��
		_lastCounter = _currentCounter;
		//��̬��ʱ����֡�ʣ�����֡�ʳ������޶�ֵ����ô�ͽ������ʱ���ӳٵ���ֹ��ѭ��Ƶ�ʹ��죻����1000�ǽ���ת��Ϊ���룬��Ϊ�������λ̫������Ȳ���
		if (_delta * 1000 < 1000.0 / fps)
			SDL_Delay((Uint32)(1000.0 / fps - _delta * 1000));
		#pragma endregion

		//��ȡ�������¼��Ա�֤������������
		while (SDL_PollEvent(&event))
			On_Input();

		//���ݸ��¼��
		On_UpdateData(_delta);

		#pragma region Render
		//ȷ����Ⱦ����ɫΪ���ڣ���͸����������RGB��ɫ��Alpha����¼ͼ���͸������Ϣ��256���Ҷȣ�
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		//ʹ���趨�Ĳ�͸����ɫ����������ڴﵽ������Ч��
		SDL_RenderClear(renderer);
		//�ھ���������׼���󣬽��о������Ⱦ��ͼ
		On_Render();
		//����Ⱦ�����ݸ��µ����ڻ�������
		SDL_RenderPresent(renderer);
		#pragma endregion
	}

	return 0;
}

GameManager::GameManager()
{
	//��ʼ��SDL���������ϵͳ����ΪSDL_Init��������0��ʾ�ɹ������Դ˴���һ���������ȡ��
	InitAssert(!SDL_Init(SDL_INIT_EVERYTHING), u8"Failed To Init SDL");
	//��ʼ��SDL_ttf�⣻TTF_Init��������0��ʾ�ɹ�
	InitAssert(!TTF_Init(), u8"Failed To Init SDL_ttf");
	//��ʼ��SDL_image��ĸ�֧�ָ�ʽ��IMG_Init�������ط���ֵ��ʾ�ɹ�������ֱ�Ӵ��뼴��
	InitAssert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG), u8"Failed To Init SDL_image");
	//��ʼ��SDL_mixer���֧�ָ�ʽ��Mix_Init�������ط���ֵ��ʾ�ɹ�
	InitAssert(Mix_Init(MIX_INIT_MP3), u8"Failed To Init SDL_mixer");
	//�򿪻�������������Mix_OpenAudio(��Ƶ������, ������Ƶ��ʽ, ������, ��Ƶ���뻺������С)
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	//����Ļ������ʾһ������Ϊ"KingdomRush-Lite"��1280x720��һ����ʽ�Ĵ��� 
	window = SDL_CreateWindow(u8"KingdomRush-Lite", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
	//��ⴰ���Ƿ��ʼ���ɹ�
	InitAssert(window, "Failed To Create Window");

	//������Ⱦ��������window�ϣ���������λʹ����������Ⱦ����
	//SDL_RENDERER_ACCELERATED��Ӳ������
	//SDL_RENDERER_PRESENTVSYNC����ֱͬ������һ��Ϊ��������Ļ˺�ѣ������ڴ�����Ƶ֡������ʾ��ˢ���ʲ�ͬ��������ʾ����
	//SDL_RENDERER_TARGETTEXTURE������ȾĿ������Ϊ����������Ⱦ����ͼƬ���ٽ�ͼƬ��Ⱦ�����ڣ�������Ƭ��ͼ������
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
	//�����Ⱦ���Ƿ��ʼ���ɹ�
	InitAssert(renderer, "Failed To Create Renderer");

	//��SDL������飬ʹ���ڴ�SDL���ڵ�ʱ�������������Ŀ�����ʾ��ѡ���б�
	//SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
}

GameManager::~GameManager()
{
	//ע������˳����������Ⱦ���봰��
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	//����ͷſ�ĳ�ʼ�����ݣ���Ϊ���������������ڿ�����ڵ�
	Mix_Quit();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

void GameManager::InitAssert(bool _flag, const char* _errMsg)
{
	//�����ʼ���ɹ�����ô���·�����ֱ�ӷ���
	if (_flag)
		return;
	//�����ʼ��ʧ�ܣ���ô����SDL����һ��������Ϣ���ڣ��ú���(��Ϣ����, ���ڱ���, ��ϸ������Ϣ, ��Ϊ˭���Ӵ���)
	//���ĸ����������NULL����ô�����ڲ���Ϊ�Ӵ��ڣ������д��봰�ڣ���ô�����ڽ���Ϊ����Ĵ��ڵ��Ӵ��ڣ����Ӵ��ڲ��ص����������޷����û�����
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"Game Init Failed", _errMsg, window);
	//��-1������������
	exit(-1);
}

void GameManager::On_Input()
{
	//������ڵ��˳���ʱ������SDL_QUIT�¼�
	if (event.type == SDL_QUIT)
		isQuit = true;
	//���ָ���ƶ��¼�
	if (event.type == SDL_MOUSEMOTION)
	{
		//��ȡ���ָ������
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
