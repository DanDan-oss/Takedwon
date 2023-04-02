#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <mysql.h>

#define DeBug

#define USER "root"
#define PASS "123456"

#define MYSQL_HOST "localhost" 
#define MYSQL_USERNAME "root" 
#define DATABASE "db_users" 

#define My_Port 9494			/* 服务端口 */
#define My_IP "47.104.222.93"	/* IP  */

#define parameter 3

struct MYSQL_ATTRIBUTE {
	char user[10];
	char passworld[10];
	char action[10];
	char mysql_name[20];
};

void strintA(char*, struct MYSQL_ATTRIBUTE*);	/* 分割命令 */
int connectA(struct MYSQL_ATTRIBUTE*);		/* 连接数据库 */

int establishA(struct MYSQL_ATTRIBUTE*);		/* 创建数据库 */
int deletesA(struct MYSQL_ATTRIBUTE*);		/* 删除数据库 */



int main()
{
	pid_t sub = fork();
	if (sub > 0)
		exit(0);
	pid_t sid = setsid();
	if (sid == 0)
	{
		printf("error: no sub promote sid ");
		return -1;
	}
	chdir("/home");
	int sub_fd = open("/dev/null", O_RDWR);
	if (sub_fd == -1)
	{
		printf("error: no such file /dev/null \n");
		return -1;
	}
	dup2(sub_fd, STDIN_FILENO);
	dup2(sub_fd, STDOUT_FILENO);
	dup2(sub_fd, STDERR_FILENO);

	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd == -1)
	{
		perror("socket");
		exit(0);
	}
	struct sockaddr_in sock_cof;
	sock_cof.sin_family = AF_INET;
	sock_cof.sin_port = htons(My_Port);
	sock_cof.sin_addr.s_addr = INADDR_ANY;
	int ret = bind(sock_fd, (struct sockaddr*)&sock_cof, sizeof(sock_cof));
	if (ret == -1)
	{
		perror("bind");
		exit(0);
	}
	ret = listen(sock_fd, 100);
	if (ret == -1)
	{
		perror("listen");
		exit(0);
	}
	while (1)
	{
		struct sockaddr_in sock_in;
		int sock_in_len = sizeof(sock_in);
		int sock_pprint = accept(sock_fd, (struct sockaddr*)&sock_in, &sock_in_len);
		if (sock_pprint == -1)
		{
			perror("accept");
			exit(0);
		}
		char* buf = (char*)malloc(sizeof(char) * 100);
		memset(buf, 0, sizeof(char) * 100);
		char* bbuf = (char*)malloc(sizeof(char) * 50);
		memset(bbuf, 0, sizeof(char) * 50);

		read(sock_pprint, (void*)&buf, sizeof(buf));
		/* user,password,data,mysql_user */
		int error = 0;
		struct MYSQL_ATTRIBUTE mysqlAa = { 0,0,0,0 };

		/*error = strintA(&buf, &mysqlA);
		if (error == -1)
		{
			sprintf(bbuf, "对不起,你传入的参数个数错误\n");
			goto Quit;
		}
		*/
		error = connectA(&mysqlAa);
		if (error == -1)
		{
			sprintf(bbuf, "数据库连接失败\n");
		}
	AQuit:
		write(sock_pprint, bbuf, strlen(bbuf));
		free(buf);
		free(bbuf);
		close(sock_pprint);
	}

	void strintA(char* buffer, struct MYSQL_ATTRIBUTE* mysqlAa)
	{

		int len = 0;
		for (int i = 0, i <= strlen(buffer), i++)
		{
			if (buffer[i] == ',')
			{
				len++;
			}
		}
		if (len != parameter)
		{
			return -1;
		}
		int start = 0;
		int ending = 0;
		len = 0;
		for (int i = 0; i < strlen(buffer), i++)
		{
			if (buffer[i] == ',')
			{
				start = ending + 1;
				ending = i;
				switch (len) {
				case:1
					mysqlAa->





				}

			}
		}


		return 0;
	}


	int connectA(struct MYSQL_ATTRIBUTE* mysqlAa)
	{
		MYSQL my_connection;
		mysql_init(&my_connection);
		if (!mysql_real_connect(&my_connection, MYSQL_HOST, MYSQL_USERNAME, NULL,
			DATABASE, 0, NULL, 0))
		{
			printf("数据库连接失败!");
			return -1;
		}
		mysql_query(&my_connection, "set names utf8");
		if (mysqlAa->)


			return &my_connection;
	}

	int establishA(struct MYSQL_ATTRIBUTE* mysqlAa)
	{

	}