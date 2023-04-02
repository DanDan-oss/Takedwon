/* Client--->TUP*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
  
#define SERVER_IP "192.168.88.128"
#define SERVER_PORT 6789

int main()
{
	int sock_fd = 0;
	struct sockaddr_in sock_addr = {0};
	char* com_buf = NULL;
	int sock_len = 0;
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == sock_fd)
	{
		perror("Create socket:");
		exit(0);
	}
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_IP, (void*)&sock_addr.sin_addr.s_addr);
	if(-1 == connect(sock_fd, (struct sockaddr*)&sock_addr, sizeof(struct sockaddr_in)))
	{
		perror("connect:");
		close(sock_fd);
		exit(0);
	}
	com_buf = (char*)malloc(1024);
	memset(com_buf, 0, 1024);
	while(1)
	{
		scanf("%s", com_buf);
		if(!strcmp(com_buf, "exit"))
			break;
		write(sock_fd, com_buf, strlen(com_buf)+1);
		memset(com_buf, 0, strlen(com_buf));
		sock_len = read(sock_fd, com_buf, 1024);
		if(-1 == sock_len || !sock_len)
		{
			perror("服务器断开链接:");
			break;
		}

		printf("%s\n请输入:", com_buf);
		memset(com_buf, 0, 1024);
	}


	if(com_buf)
	{
		free(com_buf);
		com_buf = NULL;
	}
	if(sock_fd)
	{
		close(sock_fd);
		sock_fd = 0;
	}
	return 0;
}


/* 多进程高并发  Server-->TUP */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <string.h>

#define MyPort 6789

void recycleChile(int);			// 主进程信号回调函数
void chileProcWork(int);		// 子进程通信函数

int main()
{
	pid_t pid = 0;
	int sock_fd = 0, comm_fd = 0, sock_len = 0;
	struct sockaddr_in sockaddr = { 0 };
	struct sigaction sig_tion = {0};		// 注册信号集

	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == sock_fd)
	{
		perror("Create socket:");
		exit(0);
	}
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(MyPort);
	sockaddr.sin_addr.s_addr = INADDR_ANY;
	if(-1 == bind(sock_fd, (struct sockaddr*)&sockaddr, sizeof(struct sockaddr)))
	{
		perror("bind:");
		exit(0);
	}
	if(-1 == listen(sock_fd, 129))
	{
		perror("listen:");
		exit(0);
	}

	// 注册信号,当子进程死亡时给主进程发送信号,主进程回收子进程资源
	/*signal(24, (viod(*)(int))recycleChile ); */
	sig_tion.sa_handler = (viod(*)(int))&recycleChile;
	sigemptyset(&sig_tion.sa_mask);
	sig_tion.sa_flags = 0;
	sigaction(SIGCHLD, &sig_tion, NULL);
	sock_len = sizeof(struct sockaddr_in);
	while(1)
	{
		comm_fd = accept(sock_fd, (struct sockaddr*)&sockaddr, (socklen_t*)&sock_len);
		if(comm_fd>0)
		{
			pid = fork();
			if(!pid)
				chileProcWork(comm_fd);
			close(comm_fd);
			continue;
		}
		if(-1 == comm_fd&& errno == EINTR)
			continue;
		break;
	}
}

void recycleChile(int signum)
{
	while(1)
	{
		if(waitpid(-1, NULL, WNOHANG)>0) 
			continue;
		break;
	}
	// 已死亡的子进程全部回收完毕
	return ;
}

void chileProcWork(int comm_fd)
{
	char* comm_buf = NULL;
	int buf_len = 0;
	int i = 0;

	comm_buf = (char* )malloc(1024);
	memset(comm_buf, 0, 1024);
	while(1)
	{
		buf_len = read(comm_fd, comm_buf, 1024);
		if(buf_len>0)
		{
			printf("Chile:%s\n", comm_buf);
			memset(comm_buf, 0, buf_len);
			sprintf(comm_buf, "The is Server :%d", i++);
			write(comm_fd, comm_buf, strlen(comm_buf)+1);
			memset(comm_buf, 0, strlen(comm_buf));
			continue;
		}
		if(-1 == buf_len&& errno == EINTR) 
			continue;
		break;
		
	}
	close(comm_fd);
	free(comm_buf);
	exit(0);
}


/* Server 多线程高并发---->TUP*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define	 SERVER_PORT 6789

typedef struct _THREAD_CLIENT_COMM
{
	int comm_fd;
	struct sockaddr_in sock_addr;

}THREAD_CLIENT_COMM, *PTHREAD_CLIENT_COMM;


void* thread_callback(void*);

int main()
{
	int sock_fd = 0, sock_com = 0, sock_len = 0;
	pthread_t thread_id = 0;
	struct sockaddr_in sock_addr = {0};

	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == sock_fd)
	{
		perror("Create socket:");
		exit(0);
	}
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(SERVER_PORT);
	sock_addr.sin_addr.s_addr = INADDR_ANY;
	if(-1 == bind(sock_fd, (struct sockaddr*)&sock_addr, sizeof(struct sockaddr_in)))
	{
		perror("bind:");
		exit(0);
	}
	if(-1 == listen(sock_fd, 129))
	{
		perror("listen:");
		exit(0);
	}
	sock_len = sizeof(struct sockaddr_in);
	while(1)
	{
		sock_com = accept(sock_fd, (struct sockaddr*)&sock_addr, &sock_len);
		if(sock_com>0)
		{
			PTHREAD_CLIENT_COMM thread_clicomm = (PTHREAD_CLIENT_COMM)malloc(sizeof(THREAD_CLIENT_COMM));
			thread_clicomm->comm_fd = sock_com;
			memcpy( (void*)&thread_clicomm->sock_addr, (void*)&sock_addr, sizeof(struct sockaddr));
			if( pthread_create(&thread_id, NULL, (void*(*)(void*))&thread_callback, (void*)thread_clicomm))
			{
				perror("exit->pthread_create:");
				close(sock_com);
				break;
			}
			pthread_detach(thread_id);
			continue;
		}
		if(-1 == sock_com && errno == EINTR )
			continue;
		perror("exit -->Client Comm error:");
		break;
	}
	return 0;
}

void* thread_callback(void *thread_clicomm)
{
	PTHREAD_CLIENT_COMM client_comm = thread_clicomm;
	char* comm_buf = NULL;
	int buf_len = 0;
	int i = 0;
	comm_buf = (char* )malloc(1024);
	memset(comm_buf, 0, 1024);

	while(1)
	{
		buf_len = read(client_comm->comm_fd, comm_buf, 1024);
		if(buf_len>0)
		{
			printf("Chile:%s\n", comm_buf);
			//memset(comm_buf, 0, buf_len);
			sprintf(comm_buf, "The is Server :%d", i++);
			write(client_comm->comm_fd, comm_buf, strlen(comm_buf)+1);
			//memset(comm_buf, 0, strlen(comm_buf));
			continue;
		}
		if(-1 == buf_len&& errno == EINTR) 
			continue;
		break;
	}
	free(comm_buf);
	close(client_comm->comm_fd);
	free(thread_clicomm);
	pthread_exit(NULL);
}


/* Server I/O多路转接 -->select---->TUP*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <arpa/inet.h>

#define SERVER_PORT 6789

int main()
{
	int ret_stance = 0;
	int sock_fd = 0, comm_fd = 0;
	struct sockaddr_in sock_addr = {0};		// 通信协议、IP地址、端口
	int sock_size = 0; 						// struct sockaddr大小
	char* com_buf = NULL;					// 读取通信缓冲区
	char ip_addr[20] = {0};					// 保存IP
	int fds_max = 0;						// 当前需要检测的最大描述符
	fd_set read_fds = {0}, temp_fds = {0};	// 读描述符集、临死描述符集

	sock_fd = socket(AF_INET, SOCK_STREAM, 0);  // 创建套接字(IPV4, 流式, 默认);
	if(-1 == sock_fd)
	{
		perror("create socket:");
		exit(0);
	}
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port= htons(SERVER_PORT);
	sock_addr.sin_addr.s_addr = INADDR_ANY;
	
	if(-1 == bind(sock_fd, (struct sockaddr*)&sock_addr, sizeof(struct sockaddr))) // 绑定
	{
		perror("bind:");
		exit(0);
	}
	if(-1 == listen(sock_fd, 1024))		// 监听
	{
		perror("listen:");
		exit(0);
	}
	fds_max = sock_fd;					// 设置监听描述符为当前数值最大的描述符
	FD_ZERO(&read_fds);
	FD_SET(sock_fd, &read_fds);
	com_buf = (char*)malloc(1024);
	memset(com_buf, 0, 1024);
	while(1)
	{
		temp_fds = read_fds;
		if(-1 == select(fds_max+1, &temp_fds, NULL, NULL, NULL))
		{
			printf("错误i\n");
			perror("select:");
			exit(0);
		}
		if(FD_ISSET(sock_fd, &temp_fds))	// 检测到当前通讯监听描述读端有新连接
		{
			sock_size = sizeof(struct sockaddr);
			comm_fd = accept(sock_fd, (struct sockaddr*)&sock_addr, &sock_size);
			if(-1 == comm_fd)
			{
				perror("Create Communication accept:");
				continue;
			}
			printf("新建连接,[%s:%d]分配为%d号客户端\n",inet_ntop(AF_INET, (void*)&sock_addr.sin_addr.s_addr, ip_addr, sizeof(ip_addr)),
				       	ntohs(sock_addr.sin_port),comm_fd);
			FD_SET(comm_fd, &read_fds);	 // 设置这个通讯描述符到监听集
			if(fds_max<comm_fd) 
				fds_max = comm_fd;
			continue;
		}
		// 到这里是某个通讯描述符有数据了,遍历通讯描述符集
		for(int i = 0; i<= fds_max; i++)
		{
			if(FD_ISSET(i, &temp_fds))
			{
				ret_stance = read(i, com_buf, 1024);
				if(ret_stance>0)
				{
					printf("客户端%d: %s\n",i, com_buf);
					strcat(com_buf, "Server");
					write(i, com_buf, strlen(com_buf)+1);
					memset(com_buf, 0, strlen(com_buf));
					continue;
				}else if(!ret_stance)
				{
					printf("%d号客户端关闭了链接\n",i);
					close(i);
					FD_CLR(i, &read_fds);
					continue;
				}
			}
		}
	}
	free(com_buf);
	close(sock_fd);
}


/* Server I/O多路转接 -->poll---->TUP*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include <arpa/inet.h>

#define SERVER_PORT 6789
#define SERVER_CLIENTNUM 100


typedef struct _CLIENT_INFO
{
	struct sockaddr_in sockaddr;		//
	int fd;
	unsigned int client_port;
	char client_ip[20];
}CLIENT_INFO, *PCLIENT_INFO;


int main()
{
	int ret_stance = 0;
	int sock_fd = 0, comm_fd = 0, sock_size = 0, curent_client_num=0;
	struct sockaddr_in sock_addr = {0};		// 通信协议、IP地址、端口
	char* com_buf = NULL;					// 读取通信缓冲区
	struct pollfd fds[SERVER_CLIENTNUM] = {0};
	CLIENT_INFO client_info[SERVER_CLIENTNUM] = {0};

	sock_fd = socket(AF_INET, SOCK_STREAM, 0);  // 创建套接字(IPV4, 流式, 默认);
	if(-1 == sock_fd)
	{
		perror("create socket:");
		exit(0);
	}
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port= htons(SERVER_PORT);
	sock_addr.sin_addr.s_addr = INADDR_ANY;
	
	if(-1 == bind(sock_fd, (struct sockaddr*)&sock_addr, sizeof(struct sockaddr))) // 绑定
	{
		perror("bind:");
		exit(0);
	}
	if(-1 == listen(sock_fd, 1024))		// 监听
	{
		perror("listen:");
		exit(0);
	}
	fds[0].fd = sock_fd;
	fds[0].events = POLLIN;
	com_buf = (char*)malloc(1024);
	memset(com_buf, 0, 1024);
	while(1)
	{
		curent_client_num = poll((struct pollfd*)&fds, SERVER_CLIENTNUM , -1);
		if(-1 == curent_client_num)
		{
			perror("poll:");
			sleep(500);
			continue;
		}
		if(curent_client_num>0 )
		{
			if(fds[0].revents & POLLIN)
			{
				sock_size = sizeof(struct sockaddr);
				comm_fd = accept(sock_fd, (struct sockaddr*)&sock_addr, &sock_size);
				if(-1 == comm_fd)
				{
					perror("accept: ");
					break;
				}
				curent_client_num--;
				for(int i=1; i<SERVER_CLIENTNUM; i++)
				{
					if(fds[i].fd)
						continue;
					fds[i].fd=comm_fd;
					fds[i].events = POLLIN;
					client_info[i].fd = comm_fd;
					memcpy((void*)(&client_info[i].sockaddr), (struct sockaddr*)&sock_addr, sizeof(struct sockaddr));
					inet_ntop(AF_INET, (void*)&(client_info[i].sockaddr.sin_addr.s_addr), client_info[i].client_ip, sizeof(struct sockaddr));
					client_info[i].client_port = ntohs(client_info[i].sockaddr.sin_port);

					printf("%d客户端[%s:%d]: 新连接\n", i, client_info[i].client_ip,
				       client_info[i].client_port);
					comm_fd=0;
					break;
				}
				if(comm_fd)
				{
					printf("accept comm_fd %d\n", comm_fd);
					close(comm_fd);
				}
				if(!curent_client_num)
					continue;
			}
			// 到这里是某个通讯描述符有数据了,遍历描述符
			for(int i=1; i<SERVER_CLIENTNUM; i++)
			{
				if( !(fds[i].revents& POLLIN || curent_client_num) )
					continue;
				curent_client_num--;
				ret_stance = read(fds[i].fd, com_buf, 1024);
				if(ret_stance>0)
				{
					printf("%d客户端[%s:%d]:  %s\n",i, client_info[i].client_ip, client_info[i].client_port, com_buf);
					strcat(com_buf, "Server");
					write(fds[i].fd, com_buf, strlen(com_buf)+1);
					memset(com_buf, 0, strlen(com_buf));
				}else if(!ret_stance)
				{
					printf("%d客户端[%s:%d]: 关闭了链接\n", i, client_info[i].client_ip, client_info[i].client_port);
					close(fds[i].fd);
					fds[i].fd=0;
					fds[i].events = 0;
				}
				if(curent_client_num)		
					continue;
				break;		// 当前有变化的描述符已经处理完毕,不需要再遍历下去了
			}

		}
	}
}


/* Server I/O多路转接 -->epoll---->TUP*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define SERVER_PORT 6789
#define SERVER_CLIENTNUM 3

typedef struct _CLIENT_INFO
{
	struct sockaddr_in sockaddr;
	int fd;
	unsigned int client_port;
	char client_ip[20];
}CLIENT_INFO, *PCLIENT_INFO;

int main()
{
	int ret_stance = 0;
	int sock_fd = 0, comm_fd = 0, sock_size = 0, curent_client_num=0;
	struct sockaddr_in sock_addr = {0};		// 通信协议、IP地址、端口
	CLIENT_INFO client_info[SERVER_CLIENTNUM] = {0};	// 存放客户端信息结构体
	struct epoll_event epollEvents[SERVER_CLIENTNUM]= {0};
	int epoll_root = 0;
	char* com_buf = NULL;					// 读取通信缓冲区

	sock_fd = socket(AF_INET, SOCK_STREAM, 0);  // 创建套接字(IPV4, 流式, 默认);
	if(-1 == sock_fd)
	{
		perror("create socket: ");
		exit(0);
	}
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port= htons(SERVER_PORT);
	sock_addr.sin_addr.s_addr = INADDR_ANY;
	if(-1 == bind(sock_fd, (struct sockaddr*)&sock_addr, sizeof(struct sockaddr))) // 绑定
	{
		perror("bind: ");
		exit(0);
	}
	if(-1 == listen(sock_fd, SERVER_CLIENTNUM))		// 监听
	{
		perror("listen: ");
		exit(0);
	}
	epoll_root = epoll_create(1);
	if(epoll_root<0)
	{
		perror("epoll_create: ");
		exit(0);
	}
	com_buf = (char*)malloc(1024);

	epollEvents[0].events = EPOLLIN;
	epollEvents[0].data.fd = sock_fd;
	epoll_ctl(epoll_root, EPOLL_CTL_ADD, sock_fd, (struct epoll_event*)(&epollEvents[0]));
	while(1)
	{
		curent_client_num = epoll_wait(epoll_root, (struct epoll_event*)&epollEvents[0], 
			sizeof(struct epoll_event)*SERVER_CLIENTNUM, -1);
		if(curent_client_num<0)
		{
			perror("epoll_wait: ");
			exit(0);
		}
		for(int i=0; i<curent_client_num; i++)
		{
			struct epoll_event* ep_eve = (struct sockaddr*)&epollEvents[i];
			if(ep_eve->data.fd == sock_fd)
			{
				sock_size = sizeof(struct sockaddr);
				comm_fd = accept(sock_fd, (struct sockaddr*)&sock_addr, &sock_size);
				if(-1 == comm_fd)
				{
					perror("accept: ");
					break;
				}
				for(int j=0; j<SERVER_CLIENTNUM; j++)
				{
					if(client_info[j].fd)	// 数组中的这个结构体 有描述符了
						continue;
					client_info[j].fd = comm_fd;
					memcpy((void*)(&client_info[j].sockaddr), (struct sockaddr*)&sock_addr, sizeof(struct sockaddr));
					inet_ntop(AF_INET, (void*)&(client_info[j].sockaddr.sin_addr.s_addr), 
						client_info[j].client_ip, sizeof(struct sockaddr) );
					client_info[j].client_port = ntohs(client_info[j].sockaddr.sin_port);
					printf("%d客户端[%s:%d]: 新连接\n", j+1, client_info[j].client_ip,
				      	client_info[j].client_port);
						
					ep_eve->events = EPOLLIN;
					ep_eve->data.fd = client_info[j].fd;
					epoll_ctl(epoll_root, EPOLL_CTL_ADD, client_info[j].fd, ep_eve);
					comm_fd = 0;
					break;
				}
				if(comm_fd)
				{
					printf("error: 当前服务器已达到到客户端最大连接阈值，无法新建连接\n");
					write(comm_fd, "当前服务器已满,请稍微后再试", strlen("当前服务器已满,请稍微后再试\0")+1);
					close(comm_fd);
				}
			}else
			{
				if( !(ep_eve->events&EPOLLIN))	// 检测是读事件还是其他事件
					break;

				for(int j=0; j<SERVER_CLIENTNUM; j++)
				{
					if(ep_eve->data.fd != client_info[j].fd)
						continue;
					
					ret_stance = read(ep_eve->data.fd, com_buf, 1024);
					if(-1 == ret_stance)
					{
						perror("comm read error: ");
						exit(0);
					}
					if(ret_stance>0)
					{
						printf("%d客户端[%s:%d]:  %s\n", j+1, client_info[j].client_ip, 
						client_info[j].client_port, com_buf);
						strcat(com_buf, "Server\0");
						write(ep_eve->data.fd, com_buf, strlen(com_buf)+1);
						memset(com_buf, 0, strlen(com_buf));
					}else if(!ret_stance)
					{
						printf("%d客户端[%s:%d]: 关闭了链接\n", j+1, client_info[j].client_ip, 
						client_info[j].client_port);
						close(ep_eve->data.fd);
						epoll_ctl(epoll_root, EPOLL_CTL_DEL, ep_eve->data.fd, NULL);
						client_info[j].fd = 0;
					}
					break;
				}
			}

		}
	}

	if(com_buf)
		free(com_buf);
	return 0;
}


/* Client ---->UDP*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define SERVER_IP "192.168.88.128"
#define SERVER_PORT 6789

int main()
{
	int ret_stance = 0;
	int sock_fd = 0;
	socklen_t addrlen = 0;
	struct sockaddr_in sock_send_addr = {0};		// 要发送的服务端
	struct sockaddr_in sock_recv_addr = {0};		// 接收服务端数据
	char recv_ip[20] = {0};
	char buf[100] = {"Hello World\0"};

	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);	// IPV4, 报式, 默认
	if(-1 == sock_fd)
	{
		perror("Create socket:");
		exit(0);
	}
	sock_send_addr.sin_family = AF_INET;
	sock_send_addr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_IP, (void*)&sock_send_addr.sin_addr.s_addr);

	ret_stance = sendto(sock_fd, (const void*)buf, strlen(buf)+1, 0, 
		(const struct sockaddr*)&sock_send_addr, sizeof(struct sockaddr_in));

	ret_stance = recvfrom(sock_fd, buf, sizeof(buf), 0, 
		(struct sockaddr*)&sock_recv_addr, (socklen_t*)&addrlen);

	inet_ntop(AF_INET, (const void*)&sock_recv_addr.sin_addr.s_addr, (char*)&recv_ip, sizeof(recv_ip));
	printf("Server[%s:%d] send: %s\n", recv_ip, ntohs(sock_recv_addr.sin_port), buf);
	return 0;
}


/* Server ---->UDP*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define SERVER_PORT 6789

int main()
{
	printf("1.0.0.1\n");
	int sock_fd = 0, sock_size = 0;
	struct sockaddr_in sock_addr = {0};		// 通信协议、IP地址、端口
	char buf[1024] = {0};
	char client_ip[20] = {0};

	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);  // 创建套接字(IPV4, 广播, 默认);
	if(-1 == sock_fd)
	{
		perror("create socket: ");
		exit(0);
	}
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port= htons(SERVER_PORT);
	sock_addr.sin_addr.s_addr = INADDR_ANY;
	sock_size = sizeof(struct sockaddr);
	if(-1 == bind(sock_fd, (struct sockaddr*)&sock_addr, sock_size)) // 绑定
	{
		perror("bind: ");
		exit(0);
	}
	while(1)
	{
		sock_size = recvfrom(sock_fd, buf, sizeof(buf), 0, (struct sockaddr*)&sock_addr, &sock_size);
		if(sock_size<0)
		{
			sleep(1);
			continue;
		}
		inet_ntop(AF_INET, &sock_addr.sin_addr.s_addr, client_ip, sizeof(client_ip));
		printf("[%s:%d]: %s\n", client_ip, ntohs(sock_addr.sin_port), buf);
		sprintf(buf, "[%s:%d]", client_ip, ntohs(sock_addr.sin_port));
		sendto(sock_fd, buf, strlen(buf)+1, 0, (struct sockaddr*)&sock_addr, sizeof(struct sockaddr));
		sleep(2);
	}
	close(sock_fd);
	return 0;
}


/* Server 组播---->UDP */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

#define SERVER_PORT 6789
#define MULT_ADDR "239.0.0.1"

int main()
{
	int sock_fd = 0;
	struct in_addr inaddr = {0};
	struct sockaddr_in sock_addr = {0};
	char buf[100]  = {0};
	int while_num = 0;

	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(-1 == sock_fd)
	{
		perror("create socket: ");
		exit(0);
	}
	inet_pton(AF_INET, MULT_ADDR, (void*)&inaddr.s_addr);
	if(!setsockopt(sock_fd, IPPROTO_IP, IP_MULTICAST_IF, &inaddr, sizeof(inaddr)))
	{
		perror("setsockopt: ");
		close(sock_fd);
		exit(0);
	}
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(SERVER_PORT);
	sock_addr.sin_addr.s_addr = inaddr.s_addr;
	while(1)
	{
		sprintf(buf, "Who are Dad? %d", while_num++);
		sendto(sock_fd, buf, sizeof(buf), 0, (struct sockaddr*)&sock_addr, sizeof(sock_addr));
		sleep(2);
	}
	
	close(sock_fd);
	return 0;
}

/* Client  组播---->UDP*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <string.h>

#define MULT_ADDR "239.0.0.1"
#define SERVER_PORT 6789
#define IMR_NAME "ens33"

int main()
{
	int sock_fd = 0;
	struct ip_mreqn ipmre= {0};
	struct sockaddr_in sock_addr = {0};
	char buf[128] = {0};
	
	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(-1 == sock_fd)
	{
		perror("sock_fd:");
		exit(0);
	}
	inet_pton(AF_INET, MULT_ADDR, &ipmre.imr_multiaddr.s_addr);
	ipmre.imr_address.s_addr = INADDR_ANY;
	ipmre.imr_ifindex = if_nametoindex(IMR_NAME);
	setsockopt(sock_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &ipmre, sizeof(ipmre));

	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, MULT_ADDR, &sock_addr.sin_addr.s_addr);
	if(-1==bind(sock_fd, (struct sockaddr*)&sock_addr, sizeof(sock_addr)))
	{
		perror("bind: ");
		close(sock_fd);
	}
	while(1)
	{
		recvfrom(sock_fd, buf, sizeof(buf)-1, 0, NULL, 0);
		printf("%s\n", buf);
	}

	// setsockopt(sock_fd, IPPROTO_IP, ) //离开多播组
	close(sock_fd);
	return 0;
}
