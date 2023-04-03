#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>

void start_server(int port);                                  // 启动服务
int init_listen_sock(int port);                               // 初始化端口监听描述符
void epollnode_add(int epfd, int sock_fd, int event);           // 添加描述符到epoll监听树上
void epoll_run(int epfd, int listen_fd);                        // epoll 检测,事件分发
void client_accept(int epfd, int listen_sock);                  // 监听描述符事件,有新的客户端连接
void client_datadeal(int epfd, int conn_sock);                  // 客户端数据处理
int parseRequestLine(const char* client_data);                  // 处理http/https 报文