//һ������Ŀ����ѡ������Ч�ʽϺõ�Releaseģʽ������Ϣ��ӡ������̨���е��ԣ����ַ�ʽҲ��������С����Ϸ����
//��������������"C/C++"��"��������"�µ�"���п�"����Ӧ��Ϊ"���߳�(/MT)"����ȷ����̬��������ʱ��

//��Ϸ��������
#include "../Header/Manager/Concrete/GameManager.h"

//���˺궨�壬����main������Ԥ����׶λᱻdefineΪSDL_main
#define SDL_MAIN_HANDLED

int main(int _argc, char** _argv)
{
	//������Ϸ���������ڵ���ѭ������������ȡ����ֵ
	return GameManager::Instance()->Run(_argc, _argv);
}