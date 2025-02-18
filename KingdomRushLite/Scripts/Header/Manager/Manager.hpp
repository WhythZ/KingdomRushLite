//用#ifndef预处理防止内部代码被多次包含编译
#ifndef _MANAGER_HPP_
#define _MANAGER_HPP_

//可继承单例Manager模板类实现
template <typename M>
class Manager
{
protected:
	static M* manager;                            //静态的唯一Manager类对象的指针，在外部初始化

public:
	static M* Instance();                         //静态的函数，用于获取Manager类的唯一实例指针

protected:
	Manager() = default;                          //非公开构造函数，单例模式的类不应当能被在外部创建对象
	~Manager() = default;
	Manager(const Manager&) = delete;             //无效化拷贝构造函数的调用
	Manager& operator=(const Manager&) = delete;  //无效化赋值运算符重载的调用
};

//初始化静态成员变量
template <typename M>
M* Manager<M>::manager = nullptr;

//模板函数必须在头文件中实现，不能拆分到另一个源文件中实现
template <typename M>
M* Manager<M>::Instance()
{
	//若manager未被创建（为空指针），则在堆区创建一个
	if (!manager)
		manager = new M();

	//这样我们就可以在外部通过Manager* xxxx = Manager::GetInstance();获取内部这个Manager对象的地址，而不是创建一个新的Manager
	return manager;
}

#endif
