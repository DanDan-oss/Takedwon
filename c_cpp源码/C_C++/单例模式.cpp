#include <iostream>

class Main
{
public:
	static Main* GetInstance() 
	{ 
		std::cout << "获取单例指针 " << std::endl;
		return weiyi; 
	}

private:
	static Main* weiyi;
private:
	Main();
	Main(const Main& c);
	~Main();
};

Main*  Main::weiyi = new Main;

Main::Main()
{
	std::cout << "构造函数" << std::endl;
}

Main::Main(const Main& c)
{

}

Main::~Main()
{
}

int main()
{
	Main* a = Main::GetInstance();

	system("pause");
	return 0;
}