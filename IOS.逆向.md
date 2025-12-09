# 1 工具

## 1.1 Windows

​    `爱思助手`: www.i4.cn

​    `JDK11.0.2`：[JDK11.0.2](https://mirrors.huaweicloud.com/openjdk/11.0.2/)

​     `Charles`： [Charles](https://www.charlesproxy.com/latest-release/download.do)

​     `Python-3.11.9`： [3.11.9](https://mirrors.huaweicloud.com/python/3.11.9/python-3.11.9.exe)



## 1.2 配置

### 1.2.1 Charles

Charles查看当前机器IP:  `Help` --> `Local IP addresses` 

iPhone SSL安装: `Help` --> `SSL proxying`   ->   `Install Charles Root Certificates on a mobile device or remote browser` 

Charles配置`Proxy`: 

​    设置默认端口: `Proxy`->`HTTP Prpxy` -> `port`  

​    勾选设置:  `Proxy`-> `HTTP Prpxy` ->`Support HTTP/2` 、 `Enable transparent HTTP proxying`

Charles配置`Proxy SSL`: 

​    SSL:  `Proxy`-> `SSL proxying settings` ->`SSL proxying`  -> `Enable SSL proxying`

​     SSL:  `Proxy`-> `SSL proxying settings` ->`SSL proxying`  ->  `InClude` ->  `add ` -> `Host:*,Port:*`

iPhone WiFi： 在连接的Wifi中配置HTTP代理,将Charles机器中IP和Port配置中

### 1.2.2 Python

配置pip源

```bat
Windows: C:\Users\<UserName>\pip\pip.ini
Linux： ~/.pip/pip.conf
```

```shell
[global]
index-url = https://mirrors.huaweicloud.com/repository/pypi/simple
trusted-host = mirrors.huaweicloud.com
timeout = 120
```

安装Frida

```shell
C:\Users\mohui>pip install frida
    # 安装firda-tools时会自动安装frida

C:\Users\mohui>frida --version # 判断是否安装成功frida
C:\Users\mohui>python		# 进入Python,使用import frida看能不能成功导包,说明frida库没有问题
Python 3.11.9 (tags/v3.11.9:de54cf5, Apr  2 2024, 10:12:12) [MSC v.1938 64 bit (AMD64)] on win32
Type "help", "copyright", "credits" or "license" for more information.
>>> import frida
```

```shell
# 安装项目级的frida代码提示库
C:\Users\mohui> npm i @types/frida-gum
```



### 1.2.3 iPhone

Frida-Server下载:  [frida_17.5.1_iphoneos-arm64.deb](https://github.com/frida/frida/releases/download/17.5.1/frida_17.5.1_iphoneos-arm64.deb)

```shell
C:\Users\mohui> scp -r frida_17.5.1_iphoneos-arm64.deb root@192.168.1.4:/tmp
```

登录iPhone并安装frida

```shell
C:\Users\mohui> ssh root@alpine
mohuiteki-iPhone:~ root# dokg -i /temp/frida_17.5.1_iphoneos-arm64.deb
```

windows上执行命令,验证是否能查看USB iPhone上的进程

```shell
C:\Users\mohui> frida-ps -U
```

### 1.2.4 objection

objection对frida做了进一步的封装，通过输入一系列的命令即可完成hook.安装objection之前，先安装frida和frida-tools

为了有更好的兼容性，objection的版本，最好选择当前frida版本之后更新

objection更新时间查看地址 https://pypi.org/project/objection/1.1.3/#history

frida更新时间查看地址:  [release](https://github.com/frida/frida/releases/)

```shell
C:\Users\mohui> pip install objection==1.12.2
C:\Users\mohui> objection version
objection: 1.12.2
```

```shell
常见报错：
pkg_resources.ContextualVersionConflict: (Pygments 2.11.2 (d:\soft\python386\lib\site-packages), Requirement.parse('Pygments<=2.11.1,>=1.6'), {'litecli'})
降级Pygments库到2.11.1即可
```

使用objection进行注入

```shell
#查询应用运行的pid,所有的商店安装的应用都运行在/var/containers/Bundle/Application/下面
mohuiteki-iPhone:~ root# ps aux | grep Application
mobile          2903   1.5 11.7  5330008 116280   ??  Ss   10:20AM   0:08.03 /var/containers/Bundle/Application/348756A9-A0AE-4704-9FE9-E8C138B5C5B9/WeChat.app/WeChat

# widnows进行注入
C:\Users\mohui> objection -g [PID|进程名] explore  # 低版本使用
C:\Users\mohui> objection -g [PID|进程名] start	# 高版本使用
```

```shell
2903 (run) on (iPhone OS: 12.5.7) [usb]# env		# 查看环境变量
2903 (run) on (iPhone OS: 12.5.7) [usb]# ios sslpinning disable  # hook该应用的ssl验证
2903 (run) on (iPhone OS: 12.5.7) [usb]# exit
```

frida-trace使用

```shell
C:\Users\mohui> frida-trace --help
C:\Users\mohui> frida-trace -U -i CCCrypt -p [PID]  # hook USB连接的PID的CCCrypt函数,
Instrumenting...
CCCrypt: Auto-generated handler at "C:\Users\mohui\__handlers__\libcommonCrypto.dylib\CCCrypt.js"
CCCrypt.js时hook回本地的函数的文件

C:\Users\mohui> frida-trace -U -i "CC_SHA*" -i "CC_MD*" -i "CCCrypt*" -p 2903 # 监控常见的加密函数
C:\Users\mohui> frida-trace -U -m "*[CommonCrypto* *]" -p 2903	# 监控所有CommonCrypto函数
C:\Users\mohui> frida-trace -U -m "*[MM* *crypt*]" -m "*[MM* *Crypt*]" -p 2903 #监控微信特定的加密方法
```











