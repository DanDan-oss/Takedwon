/*  epoll基于非阻塞I/O事件驱动 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>

#define MAX_EVENTS 1024					// 监听最大值
#define BUF_LEN 4096
#define SERV_PORT 6789

typedef struct _MEPOLL_EVENTS
{
    int fd;                     //  epoll监听得描述符
    struct sockaddr_in sock_addr;   //客户端IP地址
    int events;                 // 监听事件
    void* agr;                  // 泛型参数, 结构体本身-->this
    void (*callback)(int fd, int events, void* arg);    // 事件回调例程
    int status;                    // 监听状态,是否在监听红黑树上
    char buf[BUF_LEN];
    int buf_len;
    long last_avtive;           // 记录每次最后加入红黑树的时间,用户不活跃断开链接
}MEPOLL_EVENTS, *PMEPOLL_EVENTS;

void recv_data(int sock_fd, int events, void* arg);
void send_data(int sock_fd, int events, void* arg);
void wait_buf(char* recv_buf, int* buf_len);
void init_listen_socket(int epoll_efd, short port);       // 初始化监听描述符
void accept_conn(int sock_fd, int events, void* arg);     // 监听网络描述符回调例程
void server_comm(int epoll_efd, struct _MEPOLL_EVENTS *pevents, int max_event); // 服务器与客户端通信

void init_event(int epoll_efd, int sock_fd, int events, void (*callback)(int, int, void*), struct _MEPOLL_EVENTS* mev); // 初始化event
void set_event(int epoll_efd, struct _MEPOLL_EVENTS* mev, int events, void (*callback)(int, int, void*)); // 设置event属性
void delete_event(int epoll_efd, struct _MEPOLL_EVENTS* mev);     // 从红黑树删除event
void exit_server(int epoll_efd, int max_event, struct _MEPOLL_EVENTS* mev);    // 释放所有红黑树event

// void mysql_connect(char* localhost, char* rootuser, char* rootpass, short port); // mysql连接
// void mysql_adduser(char* user, char* pass, longlong time);   //  数据库添加用户
// void mysql_deleuser(char* user);            //  数据库删除用户
// void mysql_setuser(char* user, int flag,void* arg);  // 修改用户信息/时间
// int  mysql_finduser(char* user);                // 搜索用户

int g_efd;                              // epoll_create返回的红黑树根节点描述符
struct _MEPOLL_EVENTS g_events[MAX_EVENTS];

int main(int argc, char* argv[])
{
    unsigned int port = 0;
    port = SERV_PORT;
    if(argc==2)
        port = atoi(argv[1]);
    g_efd = epoll_create(MAX_EVENTS);
    if(-1 == g_efd)
    {
        printf("epoll_create in %d: %s\n", __LINE__, strerror(errno));
        exit(0);
    }
    init_listen_socket(g_efd, port);
    server_comm(g_efd, &g_events[0], MAX_EVENTS);
    return 0;
}

void init_listen_socket(int epoll_efd, short port)
{
    int sock_fd = 0;
    struct sockaddr_in addr = {0};
    int attr = 0;
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd == -1)
    {
        printf("socket fail in %d: %s\n", __LINE__, strerror(errno));
        exit(0);
    }
    attr = fcntl(sock_fd, F_GETFL);
    attr |= O_NONBLOCK;
    fcntl(sock_fd, F_SETFL, attr);
    init_event(epoll_efd, sock_fd, EPOLLIN, accept_conn, &g_events[MAX_EVENTS-1]);
    //init_event(g_events[MAX_EVENTS-1], sock_fd, accept_conn, &g_events[MAX_EVENTS-1]);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    if(-1 == bind(sock_fd, (struct sockaddr*)&addr, sizeof(addr)) )
    {
        printf("bind fail in %d: %s\n", __LINE__, strerror(errno));
        exit(0);
    }
    if(-1 == listen(sock_fd, MAX_EVENTS))
    {
        printf("listen fail in %d: %s\n", __LINE__, strerror(errno));
        exit(0);
    }
    return ;
}

void init_event(int epoll_efd, int sock_fd, int events, void (*callback)(int, int, void*), struct _MEPOLL_EVENTS* mev)
{
    struct epoll_event epev = {0};
    int flag = 0;

    epev.data.ptr = mev;
    epev.events = mev->events = events;
    mev->callback = callback;
    mev->last_avtive = time(NULL);
    if(mev->status != 1)
    {
        mev->fd = sock_fd;
        memset(&mev->sock_addr, 0, sizeof(mev->sock_addr));
        mev->agr = (void*)mev;
        mev->status = 1;
        flag = EPOLL_CTL_ADD;
    }else
        flag = EPOLL_CTL_MOD;
    if(-1 == epoll_ctl(epoll_efd, flag, mev->fd, &epev))
    {
        printf("epoll_ctl fail in %d: %s\n", __LINE__, strerror(errno));
        return;
    }
    printf("event init OK fd=%d, flag=%d, event[%0X]\n", mev->fd, flag, events);
    return;
}

void set_event(int epoll_efd, struct _MEPOLL_EVENTS* mev, int events, void (*callback)(int, int, void*))
{
    struct epoll_event epev = {0};

    if(mev->status != 1)
        return;
    epev.data.ptr = mev;
    epev.events =  events;
    if(-1 == epoll_ctl(epoll_efd, EPOLL_CTL_MOD, mev->fd, &epev))
    {
        printf("epoll_ctl fail in %d: %s\n", __LINE__, strerror(errno));
        return;
    }
    mev->events = events;
    mev->callback = callback;
    mev->last_avtive = time(NULL);
    return;
}

void delete_event(int epoll_efd, struct _MEPOLL_EVENTS* mev)
{
    struct epoll_event epeve = {0};
    if(mev->status != 1)
        return;
    epeve.data.ptr = mev;
    mev->status = 0;
    epoll_ctl(epoll_efd, EPOLL_CTL_DEL, mev->fd, &epeve);
    return;
}

void exit_server(int epoll_efd, int max_event, struct _MEPOLL_EVENTS* mev)
{
    for(int i=0; i<max_event; i++)
    {
        if(mev[i].status != 1)
            continue;
        epoll_ctl(epoll_efd, EPOLL_CTL_DEL, mev[i].fd, NULL);
        close(mev[i].fd);
        mev[i].status = 0;
    }
    return;
}
// 网络监听描述符读事件回调例程
void accept_conn(int sock_fd, int events, void* arg)
{
    struct sockaddr_in cli_addr = {0};
    socklen_t sock_len = 0;
    int conn_fd=0;
    int index=0, flag=0;
    struct _MEPOLL_EVENTS* mepeve = NULL;
    char client_ip[20] = {0};
    sock_len = sizeof(cli_addr);

    conn_fd = accept(sock_fd, (struct sockaddr*)&cli_addr, &sock_len);
    if(-1 == conn_fd && (errno!=EAGAIN || errno!=EINTR) )
    {
        printf("accept fail in %d: %s\n", __LINE__, strerror(errno));
        return ;
    }

    for(index = 0; index<MAX_EVENTS-1; index++)
        if(!g_events[index].status)
            break;
    if(index== MAX_EVENTS-1)
    {
        printf("connect limit is max[%d]\n", MAX_EVENTS);
        close(conn_fd);
        return;
    }
    if(-1 == fcntl(conn_fd, F_SETFL, O_NONBLOCK))
    {
        printf("New accept fd set fcntl fail %d: %s\n", __LINE__, strerror(errno));
        close(conn_fd);
        return;
    }
    mepeve = (struct _MEPOLL_EVENTS*)&g_events[index];
    init_event(g_efd, conn_fd, EPOLLIN, recv_data, mepeve);
    //add_event(g_efd, EPOLLIN, mepeve);
    //inet_ntop(AF_INET, &cli_addr.sin_addr.s_addr, client_ip, sizeof(client_ip));
    printf("[%s:%d]: New connect addr fd[%x], index[%d]\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port),
     conn_fd, index);
    memcpy(&mepeve->sock_addr, &cli_addr, sizeof(cli_addr));

    return ;
}

void recv_data(int sock_fd, int events, void* arg)
{
    struct _MEPOLL_EVENTS* mepeve = NULL;
    int buf_len = 0;

    mepeve = (struct _MEPOLL_EVENTS*)arg;
    buf_len = recv(sock_fd, mepeve->buf, sizeof(mepeve->buf), 0);
    if(buf_len>0)
    {
        mepeve->buf[buf_len] = '\0';
        printf("[%s:%d]: {%s}\n", inet_ntoa(mepeve->sock_addr.sin_addr), ntohs(mepeve->sock_addr.sin_port),
            mepeve->buf);
        set_event(g_efd, mepeve, EPOLLOUT, send_data);
    }else
    {
        delete_event(g_efd, mepeve);
        close(mepeve->fd);
        if(!buf_len)
            printf("[%s:%d]: close connect!\n", inet_ntoa(mepeve->sock_addr.sin_addr),
                ntohs(mepeve->sock_addr.sin_port) );
        else
            printf("[%s:%d]: recv  error [fd=%d]  %s\n", inet_ntoa(mepeve->sock_addr.sin_addr),
             ntohs(mepeve->sock_addr.sin_port), mepeve->fd, strerror(errno));
    }
    return;
}

void send_data(int sock_fd, int events, void* arg)
{
    struct _MEPOLL_EVENTS* mepeve = NULL;
    int buf_len = 0;
    mepeve = (struct _MEPOLL_EVENTS*)arg;
    //wait_buf(&mepeve->buf, &mepeve->buf_len);
    strcat(mepeve->buf, " ---server");
    buf_len = send(sock_fd, mepeve->buf, strlen(mepeve->buf)+1, 0);
    if(buf_len>0)
    {
        set_event(g_efd, mepeve, EPOLLIN, recv_data);
        //delete_event(g_efd, mepeve);
        //init_event(g_efd, sock_fd, EPOLLIN, recv_data, mepeve);
    }else
    {
        close(sock_fd);
        delete_event(g_efd, mepeve);
        printf("[%s:%d]send error in line %d: %s\n", inet_ntoa(mepeve->sock_addr.sin_addr),
        ntohs(mepeve->sock_addr.sin_port), __LINE__, strerror(errno));
    }
    return;
}

void server_comm(int epoll_efd, struct _MEPOLL_EVENTS *mepeve, int max_event)
{
    struct epoll_event events[MAX_EVENTS] = {0};
    struct _MEPOLL_EVENTS* mopeve = NULL;
    int checkpos = 0, nfd = 0;;
    long long curr_time = 0;
    while (1)
    {
        curr_time = time(NULL);

        // 超时验证,每次验证100个, 断开60秒内没有与服务器通信的客户端
        for(int i=0; i<max_event-1; i++, checkpos++)
        {
            if(checkpos == max_event-1)
                checkpos=0;
            if(mepeve[checkpos].status != 1)
                continue;
            if(curr_time- mepeve[checkpos].last_avtive>60)   // 客户端长时间不活跃,断开连接
            {
                printf("[%s:%d]: client timeout!\n", inet_ntoa(mepeve[checkpos].sock_addr.sin_addr),
                 ntohs(mepeve[checkpos].sock_addr.sin_port));
                delete_event(epoll_efd, &mepeve[checkpos]);
                close(mepeve[checkpos].fd);
            }
        }

        // 检测红黑树延迟1秒钟 ,查找有通信的描述符
        nfd = epoll_wait(epoll_efd, (struct epoll_event*)&events,
            sizeof(events)/sizeof(struct epoll_event), 1000);
        if(-1 == nfd)
        {
            printf("epoll_wait fail in %d: %s\n", __LINE__, strerror(errno));
            return ;
        }
        for(int i=0; i<nfd; i++)
        {
            mopeve = events[i].data.ptr;
            if((events[i].events &EPOLLIN) && (mopeve->events& EPOLLIN))       // 读就绪事件
                mopeve->callback(mopeve->fd, events[i].events, mopeve->agr);
            if((events[i].events &EPOLLOUT)&& (mopeve->events& EPOLLOUT))      // 写就绪事件
                mopeve->callback(mopeve->fd, events[i].events, mopeve->agr);
        }
    }

    // 退出前释放所有资源
    exit_server(epoll_efd, max_event, mepeve);
    return;
}