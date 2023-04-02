/* Client--->libevent->TUP*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <string.h>
#include <errno.h>

#define SERVER_IP "192.168.88.128"
#define SERVER_PORT 6789

void read_callback(struct bufferevent* bev, void* ctx);
void event_callback(struct bufferevent* bev, short events, void* ctx);
void cmdread_callback(struct bufferevent* bev, void* ctx);
void cmdevent_callback(struct bufferevent* bev, short events, void* ctx);

int main()
{
	struct event_base* base = NULL;
	struct bufferevent* buf_ev = NULL;
	struct bufferevent* cmd_ev = NULL;
	struct sockaddr_in server_addr = {0};

	base = event_base_new();
	if(!base)
	{
		printf("event_base_new fail in line %d: %s\n", __LINE__, strerror(errno));
		exit(0);
	}
	do
	{
		buf_ev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
		if(!buf_ev)
		{
			printf("buf_ev bufferevent_socket_new fail in line the %d: %s\n", __LINE__, strerror(errno));
			break;
		}
		cmd_ev = bufferevent_socket_new(base, STDIN_FILENO, BEV_OPT_CLOSE_ON_FREE);
		if(!cmd_ev)
		{
			printf("cmd_ev bufferevent_socket_new fail in line the %d: %s\n", __LINE__, strerror(errno));
			break;
		}
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(SERVER_PORT);
		inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr.s_addr);
		if(-1 == bufferevent_socket_connect(buf_ev, (struct sockaddr*)&server_addr, sizeof(server_addr)))
		{
			printf("bufferevent_socket_connect fail in line %d: %s\n", __LINE__, strerror(errno));
			break;
		}
		bufferevent_setcb(buf_ev, read_callback, NULL, event_callback, cmd_ev);
		bufferevent_enable(buf_ev,  EV_READ);
		bufferevent_setcb(cmd_ev, cmdread_callback, NULL, cmdevent_callback, buf_ev);
		bufferevent_enable(cmd_ev,  EV_READ);
		event_base_dispatch(base);
	} while (0);
	if(base)  event_base_free(base);
	return 0;
}

void read_callback(struct bufferevent* bev, void* ctx)
{
	char buf[124] = {0};
	int buf_len = 0;
	buf_len = bufferevent_read(bev, buf, sizeof(buf));
	printf("Server recv: %s\n", buf);
	return ;
}

void event_callback(struct bufferevent* bev, short events, void* ctx)
{
	if(events & BEV_EVENT_CONNECTED)
	{
		printf("Server connect ok...\n");
		return;
	}

	if(events & BEV_EVENT_READING)
		printf("Server close connect...\n");
	else
		printf("event_callback is exit in line %d: events=%d\n", __LINE__, events);
	bufferevent_free(bev);
	bufferevent_free((struct bufferevent*)ctx);
}

void cmdread_callback(struct bufferevent* bev, void* ctx)
{
	char buf[128] = {0};
	int buf_len = 0;
	buf_len=bufferevent_read(bev, buf, sizeof(buf));
	bufferevent_write((struct bufferevent*)ctx, buf, buf_len);
	return;
}

void cmdevent_callback(struct bufferevent* bev, short events, void* ctx)
{
	bufferevent_free(bev);
	bufferevent_free((struct bufferevent*)ctx);
}



/* Server--->libevent->TUP*/
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <string.h>
#include <errno.h>

#define SERVER_PORT 6789

struct list_entry
{
    struct list_entry* Flink;
    struct list_entry* Blink;
};

struct Linked_list
{
    struct list_entry listptr;
    struct event_base* evbase;
    struct bufferevent* evbuf;
    struct sockaddr_in client_addr;
};


struct Linked_list* find_Linked_list(const struct Linked_list* rootnode, const struct bufferevent* bev);
void add_Linked_list(struct Linked_list* rootnode, const struct bufferevent* bev, const struct sockaddr* client_addr);
void delete_Linked_list(struct Linked_list* rootnode, const struct bufferevent* bev);

void evconnlistener_callback(struct evconnlistener* , evutil_socket_t, struct sockaddr*, int, void*);
void read_callback(struct bufferevent*, void*);
void event_callback(struct bufferevent*, short, void*);

int main()
{
    struct event_base* base = NULL;
    struct evconnlistener* ev = NULL;
    struct Linked_list rootnode = {0};
    base = event_base_new();
    if(!base)
    {
        printf("event_base_new fail in lene %d: %s\n", __LINE__, strerror(errno));
        exit(0);
    }
    rootnode.client_addr.sin_family = AF_INET;
    rootnode.client_addr.sin_port = htons(SERVER_PORT);
    rootnode.client_addr.sin_addr.s_addr = INADDR_ANY;
    rootnode.evbase=base;
    rootnode.evbuf = NULL;
    rootnode.listptr.Blink = (struct list_entry*)&rootnode;
    rootnode.listptr.Flink = (struct list_entry*)&rootnode;

    ev = evconnlistener_new_bind(base, evconnlistener_callback, (void*)&rootnode, LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE, 
                        -1, (struct sockaddr*)&rootnode.client_addr, sizeof(rootnode.client_addr));
    if(!ev)
    {
        printf("evconnlistener_new_bind fail in line %d: %s\n", __LINE__, strerror(errno));
        event_base_free(base);
        exit(0);
    }
    event_base_dispatch(base);

    // 退出
    evconnlistener_free(ev);
    event_base_free(base);
}

void evconnlistener_callback(struct evconnlistener* listener, evutil_socket_t sock,
                struct sockaddr* addr, int len, void* ptr)
{
    struct Linked_list* rootnode = (struct Linked_list*)ptr;
    struct Linked_list* currnode = NULL;
    struct bufferevent* bufev = NULL;

    bufev = bufferevent_socket_new(rootnode->evbase, sock, BEV_OPT_CLOSE_ON_FREE);
    if(!bufev)
    {
        printf("bufferevent_socket_new fail in line %d: fd=%d error:%s\n", __LINE__, sock, strerror(errno));
        close(sock);
        return;
    }
    add_Linked_list(rootnode, bufev, addr);
    currnode = find_Linked_list(rootnode, bufev);
    bufferevent_setcb(bufev, read_callback, NULL, event_callback, ptr);
    bufferevent_enable(bufev, EV_READ);

    if(currnode)
        printf("[%s:%d]: connect ok\n", inet_ntoa(currnode->client_addr.sin_addr), ntohs(currnode->client_addr.sin_port));
    else
         printf("evconnlistener_callback create list fail line in %d\n", __LINE__);
    return;
}

void read_callback(struct bufferevent*bev, void* ctx)
{
    struct Linked_list* cur_node = NULL;
    char buf[128] = {0};
    int data_size = 0;

    cur_node = find_Linked_list((struct Linked_list*)ctx, bev);
    if(!cur_node)
    {
        printf("read_callback error: no find find_Linked_list line in %d\n", __LINE__);
        data_size = bufferevent_read(bev, buf, sizeof(buf));
        printf("error__ client recv: %s\n\n", buf);
        return;
    }
    data_size = bufferevent_read(bev, buf, sizeof(buf));
    printf("[%s:%d]: %s\n", inet_ntoa(cur_node->client_addr.sin_addr), ntohs(cur_node->client_addr.sin_port),
                    buf);
    sprintf(buf, "---server recv!");
    bufferevent_write(bev, buf, strlen(buf)+1);
    return;
}

void event_callback(struct bufferevent* bev, short events, void* ctx)
{
    char buf[128] = {0};
    struct Linked_list* cur_node =NULL;

    cur_node = find_Linked_list((struct Linked_list*)ctx, bev);
    if(cur_node)
    {
        if(events&BEV_EVENT_READING)
            printf("[%s:%d]: close connect;\n",inet_ntoa(cur_node->client_addr.sin_addr),
                                        ntohs(cur_node->client_addr.sin_port) );
        delete_Linked_list((struct Linked_list*)ctx, bev);
    }else
         printf("event_callback fail line in %d: events=%d\n", __LINE__, events);
    bufferevent_free(bev);
    return;
}

struct Linked_list* find_Linked_list(const struct Linked_list* rootnode, const struct bufferevent* bev)
{
    struct Linked_list* node = (struct Linked_list*)rootnode->listptr.Blink;
    while (node != rootnode && node != NULL)
    {
        if(node->evbuf == bev)
            return node;
        node = (struct Linked_list*)node->listptr.Blink;
    }
    return NULL;
}

void add_Linked_list(struct Linked_list* rootnode, const struct bufferevent* bev, const struct sockaddr* client_addr)
{
    struct Linked_list* new_node =NULL;
    if(find_Linked_list(rootnode, bev))
    {
        printf("add_Linked_list fail  bev exist in line %d\n", __LINE__);
        return ;
    }
    new_node = (struct Linked_list*)malloc(sizeof(struct Linked_list));
    memset(new_node, 0, sizeof(struct Linked_list));

    new_node->listptr.Blink = (struct list_entry*)rootnode;
    new_node->listptr.Flink = rootnode->listptr.Flink;
    ((struct Linked_list*)rootnode->listptr.Flink)->listptr.Blink = (struct list_entry*)new_node;
    rootnode->listptr.Flink = (struct list_entry*)new_node;
    new_node->evbase= rootnode->evbase;
    new_node->evbuf = (struct bufferevent*)bev;
    memcpy(&new_node->client_addr, client_addr, sizeof(struct sockaddr));
    return;
}

void delete_Linked_list(struct Linked_list* rootnode, const struct bufferevent* bev)
{
    struct Linked_list* delete_node =NULL;
    delete_node = find_Linked_list(rootnode, bev);
    if(!delete_node)
    {
        printf("delete_Linked_list fail no find bev, in line %d\n", __LINE__);
        return;
    }
    ((struct Linked_list*)delete_node->listptr.Flink)->listptr.Blink = delete_node->listptr.Blink;
    ((struct Linked_list*)delete_node->listptr.Blink)->listptr.Flink = delete_node->listptr.Flink;
    free(delete_node);
    return;
}