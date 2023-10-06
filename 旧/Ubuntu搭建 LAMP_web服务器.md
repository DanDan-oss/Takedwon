# Ubuntu搭建 LAMP_web服务器

### *`易道云学院学员练手项目`*

### *`作者：易小憨`*

## 前期准备工作

> 网站源码地址：https://cn.wordpress.org/download/
>
> 网站主题地址：https://cn.wordpress.org/themes/

> 服务器文件传输工具：https://www.xshellcn.com/xiazai.html

```shell
# 更新网络管路工具
[root@host]$ sudo apt-get install net-tools

# 更新国内源（不然一会儿下东西慢得要死)
$: cd /etc/apt/								'打开配置文件目录
$: sudo cp ./sources.list sources.list.bak	'备份镜像文件
$: sudo vi ./sources.list					'修改配置文件
'将下面的拷贝进去保存退出'
deb http://mirrors.aliyun.com/ubuntu/ bionic main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ bionic main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ bionic-security main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ bionic-security main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ bionic-updates main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ bionic-updates main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ bionic-proposed main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ bionic-proposed main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ bionic-backports main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ bionic-backports main restricted universe multiverse

# 更新配置
$: sudo apt-get update
```

## 1.安装web服务器Apache

```shell
# 安装Apache服务
$: sudo apt-get install apache2 -y


# 检测Apache服务是否开启
$: systemctl status apache2

# 查看自己的IP地址，打开windows浏览器然后输入ip地址访问网站首页
$: ifconfig								'查看ip地址'

'用windows打开浏览器，输入ip地址访问'

#这里是网站根目录 /var/www/html/   (这步可以省略)
$: cd /var/www/html/			

`Apache服务设置 （这一步我们开启Apache）`
$: service apache2 start				'开启Apache服务'
$: service apache2 stop					'停止Apache服务'
$: service apache2 restart				'重启Apache服务‘
```

## 2.搭建MySQL数据库

```shell
# 安装MySQL服务
$: sudo apt-get install mysql-server -y
$: sudo apt install mysql-client -y
$: sudo apt install libmysqlclient-dev -y

# 检查MySQL是否安装成功
$: sudo netstat -tap | grep mysql

# 数据库测试
$: mysql -u root -p

# 输入密码
	mysql>						'如果界面变成这样就说明进入了MySQL'
	mysql>create database web;	'创建网站数据库，一会儿要用 web单词可以替换自定义'
 	mysql>quit					'输入 quit 退出'


# 输入以下命令查看MySQL账户密码(可以自行修改)
$: sudo cat /etc/mysql/debian.cnf

	user = debian-sys-maint
	password = v2jTk4fe7U6HRP63
	
`mysql服务设置`
$: sudo service mysql start		'开启MySQL服务'
$: sudo service mysql stop		'停止MySQL服务'
$: sudo service mysql restart	'重启MySQL服务'

```

## 3.安装PHP

```shell
# 下载安装php脚本 
$: sudo apt install php7.2-mysql php7.2-curl php7.2-json php7.2-cgi php7.2 libapache2-mod-php7.2 -y

# 查看php版本 (可以省略这步)
$: php --version

# 修改php的配置文件
$: cd /etc/apache2/mods-enabled/	'打开配置文件所在目录'
$：vi dir.conf						'修改配置文件'
`检查修改成一样, 这个是网站进入默认访问网页`
<IfModule mod_dir.c> 
        DirectoryIndex index.php index.html index.cgi index.pl index.xhtml index.htm 
</IfModule>

# 然后重启Apache服务，让他支持php
$: sudo systemctl restart apache2

# 进入 网站根目录
$: cd /var/www/html/

# 创建php 配置网页
$: vi ./phpinfo.php

# 输入以下内容,保存退出
<?php
	echo phpinfo();
?>

# 然后用windows 在网址处输入 http://ip/phpinfo.php			注意ip输入你虚拟机的ip地址 
'这里面查看你的php的配置以自己改'

```

## 4.安装ftp文件传输服务

```shell
# 安装ftp文件传输
$ sudo apt-get install vsftpd

# 开启ftp服务，并设置开机自启动
$: sudo systemctl start vsftpd		"开启ftp"
$: sudo systemctl enable vsftpd     "设置开机自启动"

# 开启ftp端口
$ sudo ufw allow 20/tcp
$ sudo ufw allow 21/tcp
$ sudo ufw status

# 修改 ftp配置文件，保存退出
$: sudo vi /etc/vsftpd.conf			'打开ftp配置文件'
	`修改一些配置文件,这里面有的选项改成一样，其他不用管它，选线的顺序不一定是我这个顺序`
	listen=NO
	listen_ipv6=YES	
	anonymous_enable=NO				"是否允许匿名登录ftp服务器"
	local_enable=YES				"接受本地用户登录"
	write_enable=YES				"允许上传"
	dirmessage_enable=YES
	use_localtime=YES
	xferlog_enable=YES
	connect_from_port_20=YES
	chroot_local_user=YES			"限制本地用户使用其主目录"
	chroot_list_enable=YES
	chroot_list_file=/etc/vsftpd.chroot_list
	
	secure_chroot_dir=/var/run/vsftpd/empty
	pam_service_name=vsftpd
	rsa_cert_file=/etc/ssl/certs/ssl-cert-snakeoil.pem
	rsa_private_key_file=/etc/ssl/private/ssl-cert-snakeoil.key
	ssl_enable=NO
	
	`在尾部插入`
	local_root=/var/www/html	"配置ftp服务器的上传下载文件所在的目录


# 创建一个用户专门用于传输网站文件，并限制它的的权限只能访问网站目录页
$: sudo adduser yixiaohan-ftp				'后面的名字可以自定义'

# 添加允许访问ftp的用户名  (一定用root写入这个文件，否则权限不足)
$: sudo vi ./vsftpd.chroot_list
	`写入用户名`
	yixiaohan-ftp

# 修改网站根目录的文件夹权限(注意文件夹权限问题)
$: cd /var/www/html
$: sudo chmod 777  ./html

# 重启 ftp
$: sudo /etc/init.d/vsftpd restart
```





## 5.搭建我们的博客网站

```shell
# 解压我们下载的网站源码

# 打开文件传输工具
	"选择新建 ->  名称:随意 ->  主机:虚拟机ip  ->  账户:yixiaohan-ftp  --> 密码:*******

# 打开Linux下的网站根目录 /var/www/html/ 

# 将index.html删除, 把解压好的网站源码全部拖进这个网站根目录 ( /var/www/html/ )

# 使用windwos打开浏览器输入ip地址

```

```shell
# 创建网站：
	
数据库名：web				'数据库名之前建立的是定义的那个'
用户名: debian-sys-maint	 '账号密码在Linux上使用 sudo cat /etc/mysql/debian.cnf 查看'
密码: v2jTk4fe7U6HRP63

```

```shell
# 配置网站信息
网站名称: 易道云学员博客搭建							"自定义
用户名: yixiaohan									'网站管理员账户(自定义)
密码: 123456										"网站管理员密码(自定义)
电子邮件: 自定义
```



### *`易道云学院学员练手项目,未经授权不得转载或转载请注明出处`*