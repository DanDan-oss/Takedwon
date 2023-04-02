#include "server.h"
#include <sys/epoll.h>
#include <string.h>
#include <dirent.h>


void  start_server(short port)
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

int init_listen_sock(short port)
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
    printf("webServer run\n");
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
            }
            printf("%s:%d: epoll_run unknow event == %d!\n", __FILE__ , __LINE__, events[i].events);
            perror("ccc");
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
    char recv_data[4096] = {0};

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
    printf("%s", recv_data);
    // 处理get请求
    if(*recv_data=='G' &&*(recv_data+1)=='E'&&*(recv_data+2)=='T')
    {
        parseRequestLine(conn_sock, recv_data);
    }
    return;
}

int parseRequestLine(int sock_fd, const char* client_data)
{
    int request_line_len = 0;
    char* request_line_starptr = NULL;
    char* request_line_endptr = NULL;
    char request_line[2048] = {0};
    struct stat sta;

    request_line_starptr = strstr(client_data, " /");
    request_line_starptr+=2;
    request_line_endptr = strstr(request_line_starptr, " HTTP");
    request_line_len = request_line_endptr-request_line_starptr;
    memcpy(request_line, "./", 2);
    memcpy(request_line+2, request_line_starptr, request_line_len);

    if(-1 == stat(request_line, &sta))
    {
        printf("请求的资源不存在[%s]\n", request_line);
        stat("404/index.html",  &sta);
        send_resphand(sock_fd, 404, "not found", getFileType("404/index.html"), sta.st_size);
        send_respfile(sock_fd, "404/index.html");
        return 0;
    }
    if(S_ISREG(sta.st_mode))
    {
        send_respfile(sock_fd, request_line);
        return 0;
    }
    if(S_ISDIR(sta.st_mode))
    {
        send_respdir(sock_fd, request_line);
        return 0;
    }
    printf("----> %s\n", request_line);
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

void send_resphand(int sock_fd, int sta_num, const char* stat, const char* filetype, long file_size)
{   // text/plain
    char resp_line[1024] = {0};
    sprintf(resp_line, "http/1.1 %d %s \r\n", sta_num, stat);
    sprintf(resp_line+strlen(resp_line), "content-type:%s\r\n", filetype);
    sprintf(resp_line + strlen(resp_line), "content-length:%ld\r\n\r\n", file_size);
    send(sock_fd, resp_line, strlen(resp_line), 0);
    printf("%s", resp_line);
    return;
}

int send_respfile(int sock_fd, const char* filename)
{
    int fd = 0;
    int file_len = 0;
    char* file_val = NULL;
    int errp=0;

    fd  = open(filename, O_RDONLY);
    if(-1 == fd)
    {
        printf("not found server [%s]\n", filename);
        return 0;
    }
    file_len = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    send_resphand(sock_fd, 200, "ok", getFileType(filename), file_len);
    file_val = malloc(file_len);
    read(fd, file_val, file_len);
    send(sock_fd, file_val, file_len, 0);
    free(file_val);
    close(fd);
    // 打开文件
	// int fd = open(filename, O_RDONLY);
	// if (fd == -1)
	// {
	// 	perror("open");
	// 	return -1;
	// }
	// // 读文件
	// int len = 0;
	// char buf[1024];
	// while ((len = read(fd, buf, sizeof(buf))) > 0)
	// {
	// 	// 发送数据
	// 	send(sock_fd, buf, len, 0);
	// }
	// close(fd);

	// return 0;
}

int send_respdir(int sock_fd, const char* dir_path)
{
    int dir_num = 0;
    struct dirent** nalist = NULL;
    char write_data[1024] = {0};
    int buf_len = 0;
    struct stat sta = {0};
    char path[1024] = {0};

    dir_num = scandir(dir_path, &nalist, NULL, alphasort);
    if(dir_num<1)
    {
        printf("%s:%d: DIR not find!\n", __FILE__, __LINE__);
        return 0;
    }
    sprintf(write_data, "<html><head><title>%s</title></head><body><table>", dir_path);
    for(int i=0; i<dir_num; i++)
    {
        sprintf(path, "%s%s", dir_path, (char*)(nalist[i]->d_name));
        if(-1 == stat(path, &sta))
        {
            printf("%s:%d file or dirent find fail!  [%s]\n", __FILE__, __LINE__, path);
            continue;
        }
        printf("%s\n", path);
        if(S_ISREG(sta.st_mode))
        {
            sprintf(write_data+strlen(write_data), "<tr><td><a href=\"%s\">%s</a></td><td>%ld</td></tr>",
                    (char*)(nalist[i]->d_name), nalist[i]->d_name, sta.st_size);
        }
        else if(S_ISDIR(sta.st_mode))
        {
            sprintf(write_data+strlen(write_data), "<tr><td><a href=\"%s/\">%s/</a></td><td>%ld</td></tr>",
                    (char*)(nalist[i]->d_name), nalist[i]->d_name, sta.st_size);
        }
        if(strlen(write_data)>1000)
        {
            send_resphand(sock_fd, 200, "ok", ".html", strlen(write_data));
            send(sock_fd, write_data, strlen(write_data), 0);
            memset(write_data, 0, strlen(write_data)+2);
        }
    }
    sprintf(write_data+strlen(write_data), "</table></body></html>");
    send_resphand(sock_fd, 200, "ok", ".html", strlen(write_data));
    send(sock_fd, write_data, strlen(write_data), 0);
    free(nalist);
}

const char* getFileType(const char *filename)
{
	char* dot;

	// 自右向左查找‘.’字符, 如不存在返回NULL
	dot = strrchr(filename, '.');
	if (dot == NULL)
		return "text/plain; charset=utf-8";
	if (strcmp(dot, ".html") == 0 || strcmp(dot, ".htm") == 0)
		return "text/html; charset=utf-8";
	if (strcmp(dot, ".jpg") == 0 || strcmp(dot, ".jpeg") == 0)
		return "image/jpeg";
	if (strcmp(dot, ".gif") == 0)
		return "image/gif";
	if (strcmp(dot, ".png") == 0)
		return "image/png";
	if (strcmp(dot, ".css") == 0)
		return "text/css";
	if (strcmp(dot, ".au") == 0)
		return "audio/basic";
	if (strcmp(dot, ".wav") == 0)
		return "audio/wav";
	if (strcmp(dot, ".avi") == 0)
		return "video/x-msvideo";
	if (strcmp(dot, ".mov") == 0 || strcmp(dot, ".qt") == 0)
		return "video/quicktime";
	if (strcmp(dot, ".mpeg") == 0 || strcmp(dot, ".mpe") == 0)
		return "video/mpeg";
	if (strcmp(dot, ".vrml") == 0 || strcmp(dot, ".wrl") == 0)
		return "model/vrml";
	if (strcmp(dot, ".midi") == 0 || strcmp(dot, ".mid") == 0)
		return "audio/midi";
	if (strcmp(dot, ".mp3") == 0)
		return "audio/mpeg";
	if (strcmp(dot, ".ogg") == 0)
		return "application/ogg";
	if (strcmp(dot, ".pac") == 0)
		return "application/x-ns-proxy-autoconfig";

	return "text/plain; charset=utf-8";
}