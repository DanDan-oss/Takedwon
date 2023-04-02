#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#define MAX 256

long total;                     /*  ȫ�ֱ��� �����ļ���������  */

int countLines(const char* filename);       /*  ͳ���ļ�����  */
int isCode(const char* filename);           /*   �ж��Ƿ�Ϊ .c�ļ�  */
void findAllDirs(const char* path);         /*  �鿴��ǰĿ¼�µ������ļ�  */

int main()
{
    char path[MAX] = ".";

    printf("������...\n");

    findAllDirs(path);

    printf("Ŀǰ���ܹ�д�� %ld �д��룡\n\n", total);

    return 0;
}


int countLines(const char* filename)
{
    FILE* fp;
    int count = 0;
    int temp;

    /*  �ж��ļ���Ȩ��  */
    if ((fp = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "Can not open the file: %s\n", filename);       /* �޷��򿪴��ļ�  */
        return 0;
    }

    while ((temp = fgetc(fp)) != EOF)  /* ��ȡ�ļ��е�һ���ַ�, ˳���ȡ(ʹ��whileѭ�����������ļ��ж��ļ�--EOF�ǽ�β  */
    {
        if (temp == '\n')               /*  �ж��Ƿ��У�������� count++  */
        {
            count++;
        }
    }

    fclose(fp);                       /*  �ͷ�...   */

    return count;                          /*  ��������  */
}


int isCode(const char* filename)
{
    int length;

    length = strlen(filename);


    if (!strcmp(filename + (length - 2), ".c"))             /* �ж��ļ����Ƿ�Ϊ .c��β  */
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
    DIR* dp;                /* Ŀ¼ָ�� */
    struct dirent* entry;   /* Ŀ¼�ṹ��ָ�� */

    /*
     struct dirent{
        ino_t d_ino;				 ��Ŀ¼������inode
        ff_t d_off;					 Ŀ¼�ļ���ͷ����Ŀ¼������ƫ����
        signed short int d_reclen;	 d_name �ĳ���.������NULL�ַ�
        unsigned char d_type;		 d_name ��ָ���ļ�����
        har d_name[256];			 �ļ���
    };
    */

    struct stat statbuf;    /* ��ȡ�ļ�������Ϣ״̬ */

    /*
    struct stat{
        st_dev			�ļ����豸���
        st_ino			�ڵ�
        st_mode			�ļ������ͺʹ�ȡȨ��
        st_nlink		���ӵ����ļ���Ӳ��������,�ս������ļ�ֵΪ1
        st_uid			�û�ID
        st_gid			��ID
        st_rdev			�����ļ�Ϊ�豸�ļ�,��Ϊ���豸���(�豸����)
        st_size			�ļ��ֽ���(�ļ���С)
        st_blksize		���С(�ļ�ϵͳ��I/O ��������С)
        st_blocks		����
        st_atime		���һ�η���ʱ��
        st_mtime		���һ���޸�ʱ��
        st_ctime		���һ�θı�ʱ��(ָ����)
    };
    */

    if ((dp = opendir(path)) == NULL)       /* ������ǰĿ¼�������ļ�      -- DIR* opendir(const char* name);  */
    {
        fprintf(stderr, "The path %s is wrong!\n", path);        /* ����һ�������·��  */
        return;
    }
    chdir(path);                         /* ����˹���Ŀ¼           --  int chdir(const char *path );  */

    while ((entry = readdir(dp)) != NULL)    /* �ж��¸�Ŀ¼������Ƿ�Ϊ�գ� �д��������ȡ��Ŀ¼�ļ�β�򷵻�NULL.   -- struct dirent* readdir(DIR* dirp);   */
    {
        lstat(entry->d_name, &statbuf);         /* �鿴�������ļ�������  -- int lstat(const char* pathname, struct stat* buf);   */

        if (!strcmp(".", entry->d_name) || !strcmp("..", entry->d_name))          /* �Ƚ��ַ���    */
            continue;

        if (S_ISDIR(statbuf.st_mode))           /* �ж� statbuf��ָ���ļ��Ƿ�ΪĿ¼(dir)����,һ����֮ǰ�����ȵ��ú���stat(FileName, &statbuf)����lstat����ζ�Ž�FileName����ļ�����Ϣ���浽�˵�ַstatbuf��  */
        {
            findAllDirs(entry->d_name);           /*  ��ǰ�ļ������Ŀ¼�� ���ñ��������� (�ݹ�)  */
        }
        else
        {
            if (isCode(entry->d_name))             /* ������ú����жϵ�ǰ�ļ�����   */
            {
                total += countLines(entry->d_name);        /*  ������ += ��ǰ�ļ�����   */
            }
        }
    }
    chdir("..");                    /*  ������һ��Ŀ¼   */
    closedir(dp);           /*   �ͷ�...   */
}