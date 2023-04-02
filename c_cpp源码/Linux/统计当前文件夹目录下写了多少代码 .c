#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#define MAX 256

long total;                     /*  全局变量 所有文件的总行数  */

int countLines(const char* filename);       /*  统计文件行数  */
int isCode(const char* filename);           /*   判断是否为 .c文件  */
void findAllDirs(const char* path);         /*  查看当前目录下的所有文件  */

int main()
{
    char path[MAX] = ".";

    printf("计算中...\n");

    findAllDirs(path);

    printf("目前你总共写了 %ld 行代码！\n\n", total);

    return 0;
}


int countLines(const char* filename)
{
    FILE* fp;
    int count = 0;
    int temp;

    /*  判断文件打开权限  */
    if ((fp = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "Can not open the file: %s\n", filename);       /* 无法打开此文件  */
        return 0;
    }

    while ((temp = fgetc(fp)) != EOF)  /* 读取文件中的一个字符, 顺序读取(使用while循环遍历整个文件判断文件--EOF是结尾  */
    {
        if (temp == '\n')               /*  判断是否换行，如果换行 count++  */
        {
            count++;
        }
    }

    fclose(fp);                       /*  释放...   */

    return count;                          /*  返回行数  */
}


int isCode(const char* filename)
{
    int length;

    length = strlen(filename);


    if (!strcmp(filename + (length - 2), ".c"))             /* 判断文件名是否为 .c结尾  */
    {
        return 1;
    }
    else
    {
        return 0;
    }
}



void findAllDirs(const char* path)
{
    DIR* dp;                /* 目录指针 */
    struct dirent* entry;   /* 目录结构体指针 */

    /*
     struct dirent{
        ino_t d_ino;				 此目录进入点的inode
        ff_t d_off;					 目录文件开头至此目录进入点的偏移量
        signed short int d_reclen;	 d_name 的长度.不包括NULL字符
        unsigned char d_type;		 d_name 所指的文件类型
        har d_name[256];			 文件名
    };
    */

    struct stat statbuf;    /* 获取文件基础信息状态 */

    /*
    struct stat{
        st_dev			文件的设备编号
        st_ino			节点
        st_mode			文件的类型和存取权限
        st_nlink		连接到该文件的硬链接数量,刚建立的文件值为1
        st_uid			用户ID
        st_gid			组ID
        st_rdev			若此文件为设备文件,则为其设备编号(设备类型)
        st_size			文件字节数(文件大小)
        st_blksize		块大小(文件系统的I/O 缓冲区大小)
        st_blocks		块数
        st_atime		最后一次访问时间
        st_mtime		最后一次修改时间
        st_ctime		最后一次改变时间(指属性)
    };
    */

    if ((dp = opendir(path)) == NULL)       /* 遍历当前目录下所有文件      -- DIR* opendir(const char* name);  */
    {
        fprintf(stderr, "The path %s is wrong!\n", path);        /* 这是一个错误的路径  */
        return;
    }
    chdir(path);                         /* 进入此工作目录           --  int chdir(const char *path );  */

    while ((entry = readdir(dp)) != NULL)    /* 判断下个目录进入点是否为空， 有错误发生或读取到目录文件尾则返回NULL.   -- struct dirent* readdir(DIR* dirp);   */
    {
        lstat(entry->d_name, &statbuf);         /* 查看软链接文件的属性  -- int lstat(const char* pathname, struct stat* buf);   */

        if (!strcmp(".", entry->d_name) || !strcmp("..", entry->d_name))          /* 比较字符串    */
            continue;

        if (S_ISDIR(statbuf.st_mode))           /* 判断 statbuf所指向文件是否为目录(dir)类型,一般在之前都会先调用函数stat(FileName, &statbuf)或者lstat，意味着将FileName这个文件的信息保存到了地址statbuf中  */
        {
            findAllDirs(entry->d_name);           /*  当前文件如果是目录， 调用本函数本身 (递归)  */
        }
        else
        {
            if (isCode(entry->d_name))             /* 否则调用函数判断当前文件类型   */
            {
                total += countLines(entry->d_name);        /*  总行数 += 当前文件行数   */
            }
        }
    }
    chdir("..");                    /*  返回上一级目录   */
    closedir(dp);           /*   释放...   */
}