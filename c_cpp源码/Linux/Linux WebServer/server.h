#ifdef __cplusplus
    extern "C" {
#endif
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <arpa/inet.h>

void start_server(short port);                                  // 启动服务
int init_listen_sock(short port);                               // 初始化端口监听描述符
void epollnode_add(int epfd, int sock_fd, int event);           // 添加描述符到epoll监听树上
void epoll_run(int epfd, int listen_fd);                        // epoll 检测,事件分发
void client_accept(int epfd, int listen_sock);                  // 监听描述符事件,有新的客户端连接
void client_datadeal(int epfd, int conn_sock);                            // 客户端数据处理
int client_recvdata(int conn_sock, char* buf, int buf_size);                    // 获取客户端发送所有数据
int parseRequestLine(int sock_fd, const char* client_data);                  // 处理http/https 报文
int send_respfile(int sock_fd, const char* filename);
void send_resphand(int sock_fd, int sta_num, const char* stat, const char* filetype, long file_size);                                           // 发送相应报文头
int send_respdir(int sock_fd, const char* dir_path);                                  // 发送文件目录
const char* getFileType(const char *filename);

#ifdef __cplusplus
    }
#endif