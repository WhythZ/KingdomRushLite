//��#ifndefԤ�����ֹ�ڲ����뱻��ΰ�������
#ifndef _MANAGER_HPP_
#define _MANAGER_HPP_

//�ɼ̳е���Managerģ����ʵ��
template <typename M>
class Manager
{
public:
	static M* Instance();                         //��̬�ĺ��������ڻ�ȡManager���Ψһʵ��ָ��

protected:
	Manager() = default;                          //�ǹ������캯��������ģʽ���಻Ӧ���ܱ����ⲿ��������
	~Manager() = default;
	Manager(const Manager&) = delete;             //��Ч���������캯���ĵ���
	Manager& operator=(const Manager&) = delete;  //��Ч����ֵ��������صĵ���
};

//ģ�庯��������ͷ�ļ���ʵ�֣����ܲ�ֵ���һ��Դ�ļ���ʵ��
template <typename M>
M* Manager<M>::Instance()
{
	//C++11�ľֲ���̬�������̰߳�ȫ��
	static M manager;
	return &manager;
}

#endif
