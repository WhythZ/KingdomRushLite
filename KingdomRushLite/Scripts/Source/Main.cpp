//һ������Ŀ����ѡ������Ч�ʽϺõ�Releaseģʽ������Ϣ��ӡ������̨���е��ԣ����ַ�ʽ��������С����Ϸ����
//��������������"C/C++"��"��������"�µ�"���п�"����Ӧ��Ϊ"���߳�(/MT)"����ȷ����̬��������ʱ��

//��Ϸ��������
#include "../Header/Manager/Concrete/GameManager.h"

//���˺궨�壬����main������Ԥ����׶λᱻdefineΪSDL_main
#define SDL_MAIN_HANDLED

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

int main(int _argc, char** _argv)
{
    //��Emscriptenƽ̨�£�ʹ��emscripten_set_main_loop�������ͳ����Ϸ��ѭ��
    #ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(
		[]()
		{
			GameManager::Instance()->Run(0, nullptr);
		}, 0, 1
	); //0����û��ѭ��������1������ִ��
	#else
    //��Emscripten������ֱ������
    return GameManager::Instance()->Run(_argc, _argv);
    #endif
}