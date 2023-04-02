/* Client ---->本地套接字通信*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/un.h>

int main()
{
	struct sockaddr_un sock_addr = {0};
	int sock_fd = 0;
	char buf[128] = {0};

	unlink("client.sc");// 首先删除路径下得套接字文件
	sock_fd = socket(AF_LOCAL, SOCK_STREAM, 0);
	if(-1 == sock_fd)
	{
		perror("socket fail: ");
		exit(0);
	}
	sock_addr.sun_family = AF_LOCAL;
	strcpy(sock_addr.sun_path, "client.sc");
	if(-1 == bind(sock_fd, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) )
	{
		perror("bind fail: ");
		exit(0);
	}
	sock_addr.sun_family = AF_LOCAL;
	strcpy(sock_addr.sun_path, "server.sc");
	if(-1 == connect(sock_fd, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) )
	{
		perror("connect fail: ");
		exit(0);
	}
	int num = 10;
	while(num--)
	{
		sprintf(buf , "The is client send, for %d", num);
		send(sock_fd, buf, strlen(buf)+1, 0);
		recv(sock_fd, buf, sizeof(buf)-1, 0);
		printf("%s\n", buf);
		sleep(1);
	}
	close(sock_fd);
	return 0;
}


/* Server ---->本地套接字通信 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/un.h>

int main()
{
	struct sockaddr_un sock_addr = {0};
	int sockaddr_size = 0;
	int sock_fd = 0, com_fd = 0;
	char buf[128] = {0};
	int ret = 0;

	unlink("server.sc"); // 首先删除路径下得套接字文件
	sock_fd = socket(AF_LOCAL, SOCK_STREAM, 0);
	if(-1 == sock_fd)
	{
		perror("socket fail: ");
		exit(0);
	}
	sock_addr.sun_family = AF_LOCAL;
	strcpy(sock_addr.sun_path, "server.sc");
	if(-1 == bind(sock_fd, (struct sockaddr*)&sock_addr, sizeof(sock_addr)))
	{
		perror("bind fail: ");
		exit(0);
	}
	if(-1 == listen(sock_fd, 100) )
	{
		perror("listen fail: ");
		exit(0);
	}
	com_fd = accept(sock_fd, (struct sockaddr*)&sock_addr, &sockaddr_size);
	if(-1 == com_fd)
	{
		perror("accept fail: ");
		exit(0);
	}
	while (1)
	{
		ret = recv(com_fd, buf, sizeof(buf), 0);
		if(ret== -1)
		{
			perror("recv fail: ");
			break;
		}else if(!ret)
		{
			write(STDOUT_FILENO, "对方关闭了链接\n\0", strlen("对方关闭了链接\n\0"));
			break;
		}
		printf("Client: %s\n", buf);
		strcat(buf, "--Over");
		send(com_fd, buf, strlen(buf)+1, 0);
	}

	close(sock_fd);
	close(com_fd);
	return 0;
}