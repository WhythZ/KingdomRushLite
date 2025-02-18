//��#ifndefԤ�����ֹ�ڲ����뱻��ΰ�������
#ifndef _MANAGER_HPP_
#define _MANAGER_HPP_

//�ɼ̳е���Managerģ����ʵ��
template <typename M>
class Manager
{
protected:
	static M* manager;                            //��̬��ΨһManager������ָ�룬���ⲿ��ʼ��

public:
	static M* Instance();                         //��̬�ĺ��������ڻ�ȡManager���Ψһʵ��ָ��

protected:
	Manager() = default;                          //�ǹ������캯��������ģʽ���಻Ӧ���ܱ����ⲿ��������
	~Manager() = default;
	Manager(const Manager&) = delete;             //��Ч���������캯���ĵ���
	Manager& operator=(const Manager&) = delete;  //��Ч����ֵ��������صĵ���
};

//��ʼ����̬��Ա����
template <typename M>
M* Manager<M>::manager = nullptr;

//ģ�庯��������ͷ�ļ���ʵ�֣����ܲ�ֵ���һ��Դ�ļ���ʵ��
template <typename M>
M* Manager<M>::Instance()
{
	//��managerδ��������Ϊ��ָ�룩�����ڶ�������һ��
	if (!manager)
		manager = new M();

	//�������ǾͿ������ⲿͨ��Manager* xxxx = Manager::GetInstance();��ȡ�ڲ����Manager����ĵ�ַ�������Ǵ���һ���µ�Manager
	return manager;
}

#endif
