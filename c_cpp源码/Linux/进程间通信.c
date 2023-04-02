/* 创建子进程 */
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
int main()
{
	pid_t pid;
	int ret =0;
	int stance=0;
	for(int i=0; i<5; i++)
	{
		pid = fork();
		if(!pid)
			break;
	}

	if(!pid)
	{
		printf(" The child pid=%d\n", getpid());
		Sleep(15);
	}else
	{
		while(1)
		{
			Sleep(5);
			printf("The Parint pid=%d\n", getpid());
			ret = waitpid(-1, &stance, WNOHANG);
			if(ret==-1)	// 子进程已经全部死掉并且回收
				break;
			if(!ret)	// 已死掉的子进程已回收,当前还有子进程存活
				continue;
			if(ret)
			{
				printf("Child die, pid=%d, return stance=%d, form message kill %d\n", 
				ret, WIFEXITED(stance), WIFSIGNALED(stance));
				continue;
			}
		}
		printf("The child all die, Parint exit\n");
	}
	return 0;
}



/* 父子进程使用匿名管道通信 */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

int main()
{
	int fd[2] = {0};        // 创建匿名管道的读端和写端
	pid_t pid =0;           // 当前进程ID
	char* tempdata=NULL;    // 临死存放父进程从管道读出的数据
	char* bufdata = NULL; 	// 父进程从管道读出的所有数据
	char* buf=NULL;        
	int size_buf=0, temp_size = 0;

	// 创建匿名管道
	if(pipe(fd)==-1)
	{
		perror("pipe:");
		exit(0);
	}
	// 创建子进程
	pid = fork();
	if(!pid)
	{
		/* 子进程 [执行shell命令, pa aux 通过匿名管道将数据传送给父进程], 
		子进程只能往管道写一次数据psaux就会退出 */

		printf("The child pid:%d\n", getpid());
		// 重定向写文件描述符
		dup2(fd[1], STDOUT_FILENO);
		// 执行shell命令
		execlp("ps", "ps", "aux", NULL);
		// 如果继续向下执行说明execlp执行失败
		perror("The Child execlp:");
		exit(0);
	}else if(pid>0)
	{
		/* 父进程[读取子进程传送到管道中的数据,最后并全部打印出来]*/

		sleep(1);
		printf("The Parent pid:%d\n", getpid());
		tempdata = (char*)malloc(1024);
		memset(tempdata, 0, 1024);
		do
		{
			// 判断当前是否已经有读取到管道数据了,如果读到,buf指针指向数据
			memset(tempdata, 0, temp_size);
			temp_size = read(fd[0], tempdata, 1024);
			if(temp_size==0 || temp_size==-1)
				continue;
			if(size_buf&& bufdata)
				buf = bufdata;
			size_buf += temp_size;
			bufdata = (char*)malloc(size_buf+2);
			memset(bufdata, 0,size_buf+2);
			if(buf)
			{
				strcat(bufdata, buf);
				free(buf);
				buf=NULL;
			}
			strcat(bufdata, tempdata);
			sleep(1);
		}while( temp_size==1024 || !waitpid(-1, NULL, WNOHANG));

		// 子进程已经退出,并且管道数据已经读完
		wait(NULL);
		printf("\n%s\n return\n", bufdata);
		if(tempdata)
		{
			free(tempdata);
			tempdata =NULL;
		}
		if(bufdata)
		{
			free(bufdata);
			bufdata = NULL;
		}

	}else
	{
		perror("fork:");
		exit(0);
	}
	printf("exit pid:%d\n", getpid());
	return 0;
}



/* 进程间用有名管道通信 */
	// 读端
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main()
{
        int fd = 0;             // 管道文件描述符
        char* temp_data = NULL; // 临死存放从管道中读取的数据
        char* read_data = NULL; // 存放所有从管道中读取的数据
        char* buf = NULL;
        int data_size = 0, read_size = 0;
        fd = open("./pipe", O_RDONLY);
        if(fd ==-1)
        {
                if(mkfifo("./pipe", 0775) == -1)
                {
                        perror("mkfifo:");
                        exit(0);
                }
                fd = open("./pipe", O_RDONLY);
                if(fd ==-1)
                {
                        perror("open file:");
                        exit(0);
                }
        }
        printf("读端打开管道成功\n");
        temp_data = (char*)malloc(1024);
        memset(temp_data, 0,  1024);
        while(1)
        {
                read_size = read(fd, temp_data, 1024);
                if(!read_size)
                {
                        printf("当前管道中已没有数据\n");
                        break;
                }
                if(data_size && read_data)
                        buf = read_data;
                data_size += read_size;
                read_data = (char*)malloc(data_size+2);
                memset(read_data, 0, data_size+2);
                if(buf)
                {
                        strcat(read_data, buf);
                        free(buf);
                        buf = NULL;
                }
                strcat(read_data, temp_data);
                memset(temp_data, 0,  read_size);
                sleep(1);
        }
        printf("%s\n", read_data);
        if(read_data)
        {
        	
                free(read_data);
                read_data = NULL;
        }
        if(temp_data)
        {
                free(temp_data);
                temp_data = NULL;
        }
        close(fd);
        printf("读端管道关闭\n");
        return 0;
}


	// 写端
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main()
{
        int fd = 0;                             // 管道文件描述符
        char* buf_data = NULL;
        fd = open("./pipe", O_WRONLY);
        if(fd ==-1)
        {
                if(mkfifo("./pipe", 0775) == -1)
                {
                        perror("mkfifo:");
                        exit(0);
                }
                fd = open("./pipe", O_WRONLY);
                if(fd ==-1)
                {
                        perror("open file:");
                        exit(0);
                }
        }
        printf("写端打开管道成功\n");
        buf_data = (char*)malloc(2048);
        memset(buf_data, 0, 2048);
        sleep(3);
        for(int i=0; i<10; i++)
        {
                sprintf(buf_data, "tempdata%d\n", i);
                write(fd, buf_data, strlen(buf_data));
        }

        if(buf_data)
        {
                free(buf_data);
                buf_data = NULL;
        }
        close(fd);
        printf("写端管道关闭\n");
        return 0;
}



/* 父子进程通过文件/内存映射*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <string.h>

int main()
{
	pid_t pid = 0;
	int fp = 0;
	int off_size = 0;
	void* mmp_addre = NULL;
	char* buf_data = NULL;

	fp = open("./com.txt", O_RDWR);
	if(fp==-1)
	{
		fp = open("./com.txt", O_RDWR|O_CREAT, 0775);
		if(fp ==-1)
		{
			perror("open file:");
		return 0;
		}
	}
	do
	{
		off_size = lseek(fp, 0, SEEK_END);
		if(!off_size)
		{
			printf("当前文件大小为:0\n");
			off_size = 1024*4;
			lseek(fp, off_size, SEEK_END);
			write(fp, "", 1);
		}
		mmp_addre = mmap(NULL, off_size, PROT_WRITE|PROT_READ, MAP_SHARED, fp, 0);
		if(mmp_addre==MAP_FAILED)
		{
			perror("mmap:");
			break;
		}
		*(char*)mmp_addre = 0;
		pid = fork();
		if(!pid)
		{
			// 子进程
			while(1)
			{
				if(*(char*)mmp_addre==10)
				{
					printf("子进程收到退出消息了!\n");
					break;
				}
				if(*(char*)mmp_addre==1)
				{
					printf("子进程读取到的数据'%s'\n", (char*)mmp_addre+1);
					*(char*)mmp_addre=0;
					memset((char*)mmp_addre+1, 0, strlen((char*)mmp_addre+1));
				}
				sleep(2);
				continue;
			}
		}else if(pid>1)
		{
			strcpy((char*)mmp_addre+1, "父进程发送的消息?\0");
			*(char*)mmp_addre = 1;
			while(*(char*)mmp_addre!=0)
				sleep(1);
			strcpy((char*)mmp_addre+1, "父进程退出了要!\0");
			*(char*)mmp_addre = 1;
			while(*(char*)mmp_addre!=0)
				sleep(1);
			*(char*)mmp_addre = 10;
			wait(NULL);
		}else
			perror("fork:");
		break;
	}while(0);

        if(buf_data)
			free(buf_data);
        if(mmp_addre!= MAP_FAILED)
                munmap(mmp_addre, off_size);
        if(fp)
                close(fp);

        return 0;
}