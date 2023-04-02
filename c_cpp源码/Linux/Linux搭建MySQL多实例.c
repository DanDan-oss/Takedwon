#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stddef.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdbool.h>

#define MYSQL_DIR "/home/mysql/mysql_"	/* MySQLʵ��Ŀ¼  */
#define MYSQL_MAIN_IFCONGIG " /etc/apparmor.d/usr.sbin.mysqld "	/* MySQL�������ļ�  */
#define MYSQL_USER_IFCONGIG "/etc/mysql/mysql.conf.d/"			/* �û���������·��  */

typedef struct mysql_type
{
	int user_id;
	char mysql_dir[100];
	char mysql_main_cofig[100];
	char mysql_user_cofig[100];
	char mysql_user_cofig_url[100];
}*mq;

int if_user_existence(const mq a);		/* �ж��û��Ƿ��Ѵ���      --> xxxx  */
int empowerment(const mq a);			/* ���½�ʵ��Ŀ¼��Ȩ */
int mainfile_cofig(const mq a);			/* ���½��û�Ȩ��Ȩ��д�����ݿ������ļ� */
int cpuser_cofig(const mq a);			/* �����û������ļ��������Լ������ݿ�Ŀ¼�� */
int replace_user_cofig(const mq a);		/* �����滻���û�������Ϣ */
int mysql_initial(const mq a);			/* ��ʼ��MySQl���ݿ� */
int mysql_start(const mq a);			/* �����½�MySQLʵ�� */

int main(int argc, char* argv[])
{
	int error = 0;
	char* numm = argv[1];
	struct mysql_type mysql = { atoi(numm), MYSQL_DIR, MYSQL_MAIN_IFCONGIG, MYSQL_USER_IFCONGIG, MYSQL_USER_IFCONGIG };
	if (mysql.user_id > 65535 || mysql.user_id < 1000)  /* �˿� 1000-65535 */
	{
		printf("��������ȷ�����ݿ���\n");
		return -1;
	}
	strcat(mysql.mysql_dir, numm);
	strcat(mysql.mysql_user_cofig_url, "mysqld_");
	strcat(mysql.mysql_user_cofig_url, numm);
	strcat(mysql.mysql_user_cofig_url, ".cnf");

	if_user_existence(&mysql);		/*�ж��û��Ƿ��Ѵ��� */

	int mk_dir = mkdir(mysql.mysql_dir, 0777); /* �½��û��������ݿ�Ŀ¼�����ļ��и�Ȩ */
	error = empowerment(&mysql);
	if (error == -1)
	{
		printf("empowerment ��������systemʧ��\n");
		return -1;
	}
	error = mainfile_cofig(&mysql); /*  ���������ļ�д�û�Ȩ�� xx */
	if (error == -1)
	{
		printf("mainfile_cofig ��������systemʧ��\n");
		return -1;
	}
	error = system("service apparmor restart");
	if (error == -1)
	{
		printf("�ļ���Ȩ��������systemʧ��\n");
		return -1;
	}
	/*
	error = cpuser_cofig(&mysql);  ����һ���û����˵������ļ�
	if (error == -1)
	{
		printf("cpuser_cofig ��������systemʧ��\n");
		return -1;
	}
	*/
	error = replace_user_cofig(&mysql);
	if (error == -1)
	{
		printf("���������ļ�����ʧ��\n");
		return -1;
	}
	error = mysql_initial(&mysql);
	if (error == -1)
	{
		printf("mysql_initial ��������systemʧ��\n");
		return -1;
	}
	sleep(3);
	error = mysql_start(&mysql);
	if (error == -1)
	{
		printf("mysql_start ��������systemʧ��\n");
		return -1;
	}
	printf("���ݿ⣺%s ���ɳɹ�,��������, �����������������ݿ�:\n", mysql.mysql_dir);
	char test_out[100] = "mysql -S ";
	strcat(test_out, mysql.mysql_dir);
	strcat(test_out, "/mysqld.sock -P ");
	strcat(test_out, numm);
	strcat(test_out, " -u root -p");
	printf("%s\n\n", test_out);
	sleep(2);
}

int if_user_existence(const mq a)
{
	/*DIR* dp;                /* Ŀ¼ָ�� */
/*struct dirent* entry;   /* Ŀ¼�ṹ��ָ�� */
/*struct stat statbuf;    /* ��ȡ�ļ�������Ϣ״̬ */
/*if ((dp = opendir(user_file)) == NULL)       /* ������ǰĿ¼�������ļ� */
/*{
	fprintf(stderr, "The path %s is wrong!\n", user_file);        /* ����һ�������·��  */
	/*	return;
	}
*/
}

int empowerment(const mq a)
{
	char conig_url[200] = "chown -R mysql.mysql  ";
	strcat(conig_url, a->mysql_dir);
	return system(conig_url);;
}

int cpuser_cofig(const mq a)
{
	char user_cofig[150] = "sudo cp ";
	strcat(user_cofig, a->mysql_user_cofig);
	strcat(user_cofig, "mysqld.cnf  ");
	strcat(user_cofig, a->mysql_user_cofig_url);
	return system(user_cofig);
}

int mainfile_cofig(const mq a)
{
	char sys[200] = "sed -i '66i ";
	strcat(sys, a->mysql_dir);
	strcat(sys, "/ r, ");
	strcat(sys, a->mysql_dir);
	strcat(sys, "/** rwk,'");
	strcat(sys, a->mysql_main_cofig);
	/*
	sed -i '66i /home/mysql/mysql_3309/ r, /home/mysql/mysql_3309/** rwk,' /etc/apparmor.d/usr.sbin.mysqld
	*/
	return system(sys);
}

int replace_user_cofig(const mq a)
{
	char user_cofig_url[150] = "";
	strcat(user_cofig_url, a->mysql_user_cofig_url);
	char* value = (char*)malloc(sizeof(char) * 1024);
	strcat(value, "[mysqld]\n user = mysql\n pid-file = ");
	strcat(value, a->mysql_dir);
	strcat(value, "/mysqld.pid\n socket = ");
	strcat(value, a->mysql_dir);
	strcat(value, "/mysqld.sock\n port = ");
	char test[10] = { 0 };
	sprintf(test, "%d", a->user_id);
	strcat(value, test);
	strcat(value, "\n datadir = ");
	strcat(value, a->mysql_dir);
	strcat(value, "\n key_buffer_size = 16M\n myisam-recover-options = BACKUP\n log_error = ");
	strcat(value, a->mysql_dir);
	strcat(value, "/error.log\n max_binlog_size = 100M");
	FILE* fp = fopen(user_cofig_url, "w+");
	fclose(fp);
	int file_pt = open(user_cofig_url, 777);
	if (file_pt == -1)
	{
		printf("error(replace_user_cofig):���������ļ���ʧ�� %s\n", user_cofig_url);
		return -1;
	}
	int error = write(file_pt, value, strlen(value));
	if (error == -1)
		return -1;
	close(file_pt);
	free(value);
	return 0;
}

int mysql_initial(const mq a)
{
	char sys_initial[100] = "mysqld --initialize --user=mysql --basedir=/usr  --datadir=";
	strcat(sys_initial, a->mysql_user_cofig_url);
	return system(sys_initial);
}

int mysql_start(const mq a)
{
	char sys_start[100] = "mysqld_safe --defaults-file=";
	strcat(sys_start, a->mysql_user_cofig_url);
	strcat(sys_start, " --skip-grant-tables &");
	return system(sys_start);
}