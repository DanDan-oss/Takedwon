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

#define MYSQL_DIR "/home/mysql/mysql_"	/* MySQL实例目录  */
#define MYSQL_MAIN_IFCONGIG " /etc/apparmor.d/usr.sbin.mysqld "	/* MySQL主配置文件  */
#define MYSQL_USER_IFCONGIG "/etc/mysql/mysql.conf.d/"			/* 用户个人配置路径  */

typedef struct mysql_type
{
	int user_id;
	char mysql_dir[100];
	char mysql_main_cofig[100];
	char mysql_user_cofig[100];
	char mysql_user_cofig_url[100];
}*mq;

int if_user_existence(const mq a);		/* 判断用户是否已存在      --> xxxx  */
int empowerment(const mq a);			/* 给新建实例目录赋权 */
int mainfile_cofig(const mq a);			/* 将新建用户权限权限写进数据库配置文件 */
int cpuser_cofig(const mq a);			/* 拷贝用户配置文件并放入自己的数据库目录下 */
int replace_user_cofig(const mq a);		/* 批量替换新用户配置信息 */
int mysql_initial(const mq a);			/* 初始化MySQl数据库 */
int mysql_start(const mq a);			/* 启动新建MySQL实例 */

int main(int argc, char* argv[])
{
	int error = 0;
	char* numm = argv[1];
	struct mysql_type mysql = { atoi(numm), MYSQL_DIR, MYSQL_MAIN_IFCONGIG, MYSQL_USER_IFCONGIG, MYSQL_USER_IFCONGIG };
	if (mysql.user_id > 65535 || mysql.user_id < 1000)  /* 端口 1000-65535 */
	{
		printf("请输入正确的数据库编号\n");
		return -1;
	}
	strcat(mysql.mysql_dir, numm);
	strcat(mysql.mysql_user_cofig_url, "mysqld_");
	strcat(mysql.mysql_user_cofig_url, numm);
	strcat(mysql.mysql_user_cofig_url, ".cnf");

	if_user_existence(&mysql);		/*判断用户是否已存在 */

	int mk_dir = mkdir(mysql.mysql_dir, 0777); /* 新建用户创建数据库目录并对文件夹赋权 */
	error = empowerment(&mysql);
	if (error == -1)
	{
		printf("empowerment 函数调用system失败\n");
		return -1;
	}
	error = mainfile_cofig(&mysql); /*  往主配置文件写用户权限 xx */
	if (error == -1)
	{
		printf("mainfile_cofig 函数调用system失败\n");
		return -1;
	}
	error = system("service apparmor restart");
	if (error == -1)
	{
		printf("文件赋权函数调用system失败\n");
		return -1;
	}
	/*
	error = cpuser_cofig(&mysql);  拷贝一份用户个人的配置文件
	if (error == -1)
	{
		printf("cpuser_cofig 函数调用system失败\n");
		return -1;
	}
	*/
	error = replace_user_cofig(&mysql);
	if (error == -1)
	{
		printf("个人配置文件生成失败\n");
		return -1;
	}
	error = mysql_initial(&mysql);
	if (error == -1)
	{
		printf("mysql_initial 函数调用system失败\n");
		return -1;
	}
	sleep(3);
	error = mysql_start(&mysql);
	if (error == -1)
	{
		printf("mysql_start 函数调用system失败\n");
		return -1;
	}
	printf("数据库：%s 生成成功,并已启动, 请输入命令进入此数据库:\n", mysql.mysql_dir);
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
	/*DIR* dp;                /* 目录指针 */
/*struct dirent* entry;   /* 目录结构体指针 */
/*struct stat statbuf;    /* 获取文件基础信息状态 */
/*if ((dp = opendir(user_file)) == NULL)       /* 遍历当前目录下所有文件 */
/*{
	fprintf(stderr, "The path %s is wrong!\n", user_file);        /* 这是一个错误的路径  */
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
		printf("error(replace_user_cofig):个人配置文件打开失败 %s\n", user_cofig_url);
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