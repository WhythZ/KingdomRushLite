//用#ifndef预处理防止内部代码被多次包含编译
#ifndef _MANAGER_HPP_
#define _MANAGER_HPP_

//可继承单例Manager模板类实现
template <typename M>
class Manager
{
public:
	static M* Instance();                         //静态的函数，用于获取Manager类的唯一实例指针

protected:
	Manager() = default;                          //非公开构造函数，单例模式的类不应当能被在外部创建对象
	~Manager() = default;
	Manager(const Manager&) = delete;             //无效化拷贝构造函数的调用
	Manager& operator=(const Manager&) = delete;  //无效化赋值运算符重载的调用
};

//模板函数必须在头文件中实现，不能拆分到另一个源文件中实现
template <typename M>
M* Manager<M>::Instance()
{
	//C++11的局部静态变量是线程安全的
	static M manager;
	return &manager;
}

#endif
