#include <iostream>

char* calculation(const char* source, const char* target)
{
	for (int i = 0; source[i]; i++)
	{
		if (source[i] == target[0]) 
		{
			bool seek{ true };
			int x{ 0 };
			for (x = 0; target[x]; x++)
			{
				if (source[x+i] != target[x])
				{
					seek = false;
					break;
				}
			}
			if (seek == true)
				return (char*)&source[i];
		}
	}
	std::cout << "寻址错误\n";
	return nullptr;
}

int main(int argc,char *argv[])
{
	//const char* argv[]{ {" id:1213"},{"pass:1231"},{"guojia:1123"} };
	/*
	for (int i = 0; i < argc; i++)
		std::cout << argv[i] << std::endl;
	*/
	char* id{}, * pass{}, * guojia{};
	const char* id_key{ "id:" };
	const char* pass_key{ "pass:" };
	const char* guojia_key{ "guojia:" };


		if (id == nullptr)
		{
			id = calculation(argv[1], id_key);
		}
		if (pass == nullptr)
		{
			pass = calculation(argv[2], pass_key);
		}
		if (guojia == nullptr)
		{
			guojia = calculation(argv[3], guojia_key);
		}

	if (id==nullptr && pass == nullptr&& guojia == nullptr)
	{
		std::cout << "未输入参数或传参失败.......\n";
	}
	else 
	{
		std::cout << "id = " << id+3 << "   pass = " << pass+5 << "   guojia = " << guojia+7 << (char)10;
	}

	system("pause");

	// argv[]为保存命令行参数的字符串指针，其中第0个参数是程序的全名，以后的参数为命令行后面跟的用户输入的参数



	return 0;
}