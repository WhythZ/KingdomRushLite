////
////1������Ŀ����ѡ������Ч�ʽϺõ�Releaseģʽ������Ϣ��ӡ������̨���е��ԣ����ַ�ʽҲ��������С����Ϸ����
////2������������"C/C++"��"��������"�µ�"���п�"����Ӧ��Ϊ"���߳�(/MT)"����ȷ����̬��������ʱ��
////

//�ļ���д
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
//���ڽ���JSON�ļ�
#include <cJSON.h>
//��Ϸ��������
#include "../Header/Manager/Concrete/GameManager.hpp"

//���˺궨�壬����main������Ԥ����׶λᱻdefineΪSDL_main
#define SDL_MAIN_HANDLED

int main(int _argc, char** _argv)
{
	//������Ϸ���������ڵ���ѭ������������ȡ����ֵ
	return GameManager::Instance()->Run(_argc, _argv);
}