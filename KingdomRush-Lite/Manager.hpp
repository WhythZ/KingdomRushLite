//�ɼ̳е���Managerģ����ʵ�֣���#ifndefԤ�����ֹ�ڲ����뱻��ΰ�������
#ifndef _MANAGER_HPP_
#define _MANAGER_HPP_

template<typename T>
class Manager
{
public:
	//��̬�ĺ��������ڻ�ȡManager���Ψһʵ��ָ��
	static T* GetInstance()
	{
		//��managerδ��������Ϊ��ָ�룩�����ڶ�������һ��
		if (!manager)
		{
			manager = new T();
		}

		//�������ǾͿ������ⲿͨ��Manager* xxxx = Manager::GetInstance();��ȡ�ڲ����Manager����ĵ�ַ�������Ǵ���һ���µ�Manager
		return manager;
	}

protected:
	//��̬��ΨһManager������ָ�룬���ⲿ��ʼ��
	static T* manager;

protected:
	/*����������Ҫ����������ɵ��ã��ⲿ���ɷ���*/
	//���캯��������ģʽ���಻Ӧ���ܱ����ⲿ��������
	Manager() = default;
	//��������
	~Manager() = default;
	//�������캯���ĵ�����Ч
	Manager(const Manager&) = delete;
	//�����=�����صĵ�����Ч
	Manager& operator=(const Manager&) = delete;
};

//���ⲿ��ʼ����̬��Ա����
template<typename T>
T* Manager<T>::manager = nullptr;

#endif
