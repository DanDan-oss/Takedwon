#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <string.h>

#define SO_REUSEPORT	15

void start_server(int port)
{
    int listen_fd = 0;
    int epfd = 0;

    listen_fd = init_listen_sock(port);
    if(-1 == listen_fd)
        exit(0);
    epfd = epoll_create(1024);
    if(-1 == epfd)
    {
        close(listen_fd);
        exit(0);
    }
    epollnode_add(epfd, listen_fd, EPOLLIN);
    epoll_run(epfd, listen_fd);
}

int init_listen_sock(int port)
{
    int listen_fd = 0;
    struct sockaddr_in server_addr = {0};
    int option= 1;
    if(port<0)
    {
        printf("%s:%d: start server port is error\n", __FILE__, __LINE__);
        return -1;
    }
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == listen_fd)
    {
        printf("%s:%d: soctet fail!\n", __FILE__ , __LINE__ );
        return -1;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEPORT, &option,sizeof(option) );
    if(-1 == bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) )
    {
        printf("%s:%d: bind fail!\n", __FILE__ , __LINE__);
        close(listen_fd);
        return -1;
    }
    if(-1 == listen(listen_fd, 100))
    {
        printf("%s:%d: listen fail!\n", __FILE__ , __LINE__);
        close(listen_fd);
        return -1;
    }

    return listen_fd;
}

void epollnode_add(int epfd, int sock_fd, int event)
{
    struct epoll_event epeve = {0};

    epeve.events = event;
    epeve.data.fd = sock_fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sock_fd, &epeve);
    return;
}

void epoll_run(int epfd, int listen_fd)
{
    struct epoll_event events[50] = {0};
    int sock_num = 0;           // 有变化的描述符数量
    int cur_fd = 0;

    while (1)
    {
        sock_num = epoll_wait(epfd, (struct epoll_event*)&events, sizeof(events)/sizeof(struct epoll_event), -1);
        for(int i=0; i<sock_num; i++)
        {
            if(listen_fd == events[i].data.fd)
            {
                client_accept(epfd, listen_fd);
                continue;
            }
            else if(events[i].events == EPOLLIN)
            {
                client_datadeal(epfd,events[i].data.fd);
                continue;
            }else if( events[i].events==25)
            {
                printf("%s:%d: epoll_run 检测到可能是Windows Socket关闭 == %d!\n", __FILE__ , __LINE__, events[i].events);
                epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                close(events[i].data.fd);
                continue;
            }
            printf("%s:%d: epoll_run unknow event == %d!\n", __FILE__ , __LINE__, events[i].events);
        }
    }

}

void client_accept(int epfd, int listen_sock)
{
    int conn_sock = 0;
    struct sockaddr_in client_addr = {0};
    int sockaddr_size =0;
    sockaddr_size = sizeof(client_addr);

    conn_sock = accept(listen_sock, (struct sockaddr*)&client_addr, &sockaddr_size);
    if(-1 == conn_sock)
    {
        printf("%s:%d: new client accept fail!\n", __FILE__ , __LINE__);
        return;
    }
    printf("[%s:%d]: connect\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    epollnode_add(epfd, conn_sock, EPOLLIN|EPOLLET);
    return;
}

void client_datadeal(int epfd, int conn_sock)
{
    int recv_datalen = 0;
    char recv_data[1024] = {0};

    recv_datalen = recv(conn_sock, recv_data, sizeof(recv_data), 0);
    if(-1 == recv_datalen || !recv_datalen)
    {
        if(-1 == recv_datalen)
            printf("%s:%d: recv client fail!\n", __FILE__ , __LINE__);
        else
            printf("client close connect\n");
        epoll_ctl(epfd, EPOLL_CTL_DEL, conn_sock, NULL);
        close(conn_sock);
        return;
    }
    parseRequestLine(recv_data);
    return;
}

int parseRequestLine(const char* client_data)
{
    int Request_linelen = 0;
    int Request_headlen = 0;
    char* Request_line_end = NULL;
    char* Request_head_end = NULL;
    char Request_line[1024] = {0};
    char Request_head[1024] = {0};
    Request_line_end = strstr(client_data, "\r\n");
    if(Request_line_end)
    {
        Request_linelen = (int)(Request_line_end - client_data+2);
        Request_linelen =(Request_linelen >1024? 1024 : Request_linelen);
        memcpy(Request_line, client_data, Request_linelen);
        Request_head_end = strstr(Request_line_end+2, "\r\n\r\n");
    }
    if(Request_head_end && Request_line_end)
    {
        Request_headlen = (int)(Request_head_end- Request_line_end);
        Request_headlen = (Request_headlen>1024? 1024: Request_headlen);
        memcpy(Request_head, (char*)(Request_line_end+ 2), Request_headlen);
    }
    printf("HTTP: \n%s%s", Request_line, Request_head);
    return 1;
}

// int parseRequestLine(const char* client_data)
// {
//     int Request_linelen = 0;
//     int Request_headlen = 0;
//     const char* Request_headptr = NULL;
//     const char* Request_endptr = NULL;
//     char* bufptr = NULL;
//     char Request_line[1024] = {0};
//     char Request_head[1024] = {0};

//     bufptr = Request_line;
//     memcpy(bufptr, "  ", 2);
//     bufptr+=2;
//     Request_headptr = client_data;
//     Request_endptr = strstr(Request_headptr, "\r\n");
//     if(Request_endptr)
//     {
//         Request_linelen = Request_endptr+2-Request_headptr;
//         Request_linelen =(Request_linelen >1024? 1024 : Request_linelen);
//         memcpy(bufptr, Request_headptr, Request_linelen);
//         bufptr = Request_head;
//         while (1)
//         {
//             Request_endptr +=2;
//             Request_headptr = Request_endptr;
//             Request_endptr = strstr(Request_headptr, "\r\n");
//             if(Request_endptr == Request_headptr)
//                 break;
//             memcpy(bufptr, "  ", 2);
//             bufptr+=2;
//             Request_headlen = Request_endptr+2-Request_headptr;
//             Request_headlen =(Request_headlen >1024? 1024 : Request_headlen);
//             if(Request_headlen+bufptr >= Request_head+1024)
//             {
//                 memcpy(bufptr, Request_headptr, Request_head+1024-bufptr);
//                 break;
//             }
//             memcpy(bufptr, Request_headptr, Request_headlen);
//             bufptr+=Request_headlen;
//             continue;
//         }
//     }

//     printf("Request_line:\n%s", Request_line);
//     printf("Request_head:\n%s", Request_head);
//     return 1;
// }
