//可继承单例模板类实现，用#ifndef预处理防止内部代码被多次编译
#ifndef _MANAGER_H_
#define _MANAGER_H_
class Manager
{
public:
	//静态的函数，用于获取Manager类的唯一实例指针
	static Manager* GetInstance()
	{
		//若manager未被创建（为空指针），则在堆区创建一个
		if (!manager)
		{
			manager = new Manager();
		}

		//这样我们就可以在外部通过Manager* xxxx = Manager::GetInstance();获取内部这个Manager对象的地址，而不是创建一个新的Manager
		return manager;
	}

	//析构函数
	~Manager() = default;

private:
	//静态的唯一Manager类对象的指针，在外部初始化：Manager* Manager::manager = nullptr;
	static Manager* manager;

private:
	//私有的构造函数，单例模式的类不应当能被在外部创建对象
	Manager() = default;
};
#endif // !_MANAGER_H_
