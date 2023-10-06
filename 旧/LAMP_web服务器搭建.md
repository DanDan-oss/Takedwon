## Windows Web搭建

> Apache 下载地址 :<u>[`https://www.apachehaus.com/cgi-bin/download.plx`](https://www.apachehaus.com/cgi-bin/download.plx)</u>
>
> PHP 下载地址: <u>[`https://www.php.net/downloads.php`](https://www.php.net/downloads.php)</u>
>
> MySQL 下载地址: 

###    安装配置Apache

> 以下`httpd.conf`行号仅供参考
>
> 改完配置检查Apache是否配置出错 `httpd -t`并重启Apache

```bash
# 1.解压Apache
# 2.打开Apache根目录下的 ./conf/httpd.conf修改Apache文件配置定义默认路径
 39:  Define SRVROOT "C:/web/httpd-2.4.41-o111c-x64-vc15-r2/Apache24"
 
# 3.以`管理员`权限运行CMD,并切换cmd到Apache根目录下的 ./bin/
 $	cd C:/web/httpd-2.4.41-o111c-x64-vc15-r2/Apache24/bin/
 
# 4.安装Apache服务, -n指定服务名
 $	httpd -k install -n "Apache"
 
# 5.检查Apache服务是否生效
 $	httpd -t      # --Syntax OK
 
# 6.打开Apache根目录下的 ./conf/httpd.conf
	# 1).添加'服务器主机地址~端口
 233:  ServerName www.mohui.xyz:80
 234:  Servername localhost:80
	# 2).修改网站管理员邮箱(虚拟主机可以不用改)
 223:  ServerAdmin 123456@qq.com
	# 3).添加配置Apache服务监听端口
 67:   Listen 80		#-- http
 68:   Listen 443		#-- https
	# 4).配置网站根目录并设置访问权限
 256:  DocumentRoot "C:/www"
 257:  <Directory "C:/www">
	# 5).设置默认文档并关闭目录浏览
 290:  DirectoryIndex index.html index.php
 270:  Options FollowSymLinks		#--关闭目录浏览
 
```

## Apache支持PHP

```bash
# 1.解压PHP并打开Apache主配置文件./conf/httpd.conf
# 2.在Apache模块加载项后面 (Listen 80端口监听) 添加PHP的加载模块修改php MIMEType(文件类型) 
	# 1)
 191:  #PHP 处理模块
 192:  LoadModule php7_module "C:/web/php-7.4.2-Win32-vc15-x64/php7apache2_4.dll"
	# 2)在 <ifMoodule mime_module>节点 下添加 php MIMEType
 440:  AddType application/x-httpd-php .php

```

