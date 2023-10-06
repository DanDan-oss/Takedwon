# QT开发

```c++
'Qt基础快捷键'
ctrl + R //编译运行
ctrl + B //编译
ctrl + / //注释
ctrl + F //查找 ESC 退出
ctrl + i //自动对齐
ctrl + shift +方向键 //整行移动
F4 //文件切换 ( cpp 和 h)
```

```c++
'头文件'
#include <QApplication> /* Qt的所有基础头文件 */
#include <QWidget> /* 窗口控件的父类头文件[Widget] */
#include <QPushButton> //常用按钮控件
#include <QDebug> //命令控制
#include <QMenuBar> /* 菜单栏控件 */
#include <QStatusBar> /* 工具栏 */
#include <QLabel> /* 文本信息 */
#include <QDockWidget> /* 铆接部件 */
#include <QTextEdit> /* 文本编辑控件 */
```

```c++
'QT数据类型转换'
temp.toUtf8().data(); //Qstring转char*调用toUtf8()转换为 QBtyeArray,再调用data()转换为
/* Qstring 默认会加双引号 */
char* b = a.toUtf8().data();
QString::number(a); //int a 转换为QString类型
```

## 1.2 lanbda表达式

```c++
"lambda" /* lambda表达式创建匿名函数c++11新特性 Qt5以下需在.pro文件声明 [CONFIG +=
c++1] */
int num = []()->int{ return 100; }();
[ ] /* 不引入任何任何函数对象参数 */
[=] /* 引入表达式外部变量, 地址传递 */
[&] /* 引用传递外部变量,在匿名函数内部使用 */
[a] /* 按a值的方式传递, 变量只读状态 */
[&a] /* 将a按引用传递 */
[a, &b] /* a按值传递, b按引用方式传递 */
[=, a, &b] /* 除了a使用值传递,b使用引用传递,其它都按引入变量(地址)传递
*/
"mutable关键字"
/*使用mutable关键字可以修改lambda按值传参[a]使拷贝的数值*/
connect(btn, &QPushButton::clicked, [a]()mutable { int a= 0; })
```

## 1.3 信号和槽

> 当信号或者槽发生重载时, 需要使用函数指针明确指
>
> 向 信号和槽的参数个数并不一定一致,信号的参数可以多余槽,但槽函数的参数个数不能多余信号 信号和槽的参数必须一一对应

```c++
"信号和槽" /* 绑定控件 */
/* connect(信号的发送者[指针], 发送的信号[函数入口地址], 信号的接收者[指针], 信号处理[槽函数
入口])*/
connect(btn, &QPushButton::clicked, this, &QWidget::close); //链接信号
&QPushButton::clicked //点击信号
&QPushButton::pressed //按下信号(鼠标按下)
&QPushButton::released //释放(鼠标抬起)
&QPushButton::triggered //触发
&QPushButton::toggled //开关状态
&QPushButton::function
connect(btn, SIGNAL(clicked()), this, SLOT(close()));
/* QT4版本写法,Qt5兼容写法，[致命缺点参数类型不做检查] */
connect(btn, SIGNAL(clicked(QString)), this, SLOT(function(QString))); /* 有
参*/
coonetc(btn, &QPushButton::clicked, [=](){ int a= 0; }) /* 匿名函数 */
/* 当connect链接时,内部会将按钮进入锁状态[只读],用&传参会报错,使用= */
    
'槽函数' /* 当绑定多个槽函数时,槽函数的执行顺序时随机的,不能控制 */
&QWidget::function
close() // 关闭窗口的槽信号
    
# 断开信号和槽的链接
disconnect(btn, &QPushButton::clicked, this, &QWidget::close); //断开链接

```

## 1.4 QMainWindow基础控件

### 1.4.1 主窗口

```c++
# 主窗口 Widget
resize(600, 400); /* 重置窗口大小[没固定,可拖拽大小] */
this->setWindowTitle("title"); /* 设置窗口标题 */
this->setFixedSize(x, y); /* 固定窗口大小 */
this->setFixedHeight(600); /* 设置固定的高 */
this->setFixedWidth(400); /* 设置固定的宽*/
```

### 1.4.2 按钮

```c++
# 设置按钮
	QPushButton *btn1 = new QPushButton("Text", this);
	/* 创建一个按钮[设置按钮显示名字,按钮的父窗口] */
	{
		QPushButton *btn = new QPushButton;
		btn->setParent (this); /* 设置按钮的父窗口,如果是结构函数内调用可以用this代替 */
		btn->setText("text"); /* 设置按钮的显示文本 */
		btn->move(x, y); /* 移动按钮位置,设置坐标 */
	}
```

### 1.4.3 菜单栏

```c++
#include <QmenuBar>
	QmenuBar *bar = menuBar(); //菜单栏, 只能有一个 #include<QmenuBar>
	this->setMenuBar(bar) //把菜单栏设置窗口
	QMenu *fileMenu = bar -> addMenu("name"); //添加菜单栏
	QAction *action1 = fileMenu -> addAction("name"); //添加菜单项
	qm1->addSeparator(); //在菜单项之间添加分割线
```

### 1.4.4 工具栏

```c++
#include <QToolBar>
	QToolBar *toolBar = new QToolBar(this); //工具栏, 可以创建多个工具,用new创建需指定父窗口
	addToolBar(Qt::LeftToolBarArea, toolBar); //给某个工具栏设置窗口,并设置停靠方向
		toolBar->setMoveable(false); //设置是否允许拖拽移动
		toolBar->setAllowedAreas(Qt::LeftToolBarArea | Qt::ReightToolBarArea);
						/* 设置只允许工具栏停靠的位置 (左 | 右 | 上 | 下) */
		toolBar->setFloatable(false); //设置工具栏是否允许浮动
		toolBar->addAction("name"); //工具栏新建菜单项
		toolBar->addSeparator(); //工具栏菜单项之间添加分割线

```

### 1.4.5 状态栏

```c++
#include <QStatusBar>
/* 状态栏 只能有一个 */
	QStatusBar *stBar = statusNar();
	setStatusBar(stBar); //状态栏设置窗口,
	QLable *label1 = new QLabel("text"); //创建新状态栏信息 #include <QLabel (文本控件)>
		stBar->addWidget(label1); //将文本信息绑定到状态栏中 (从左往右添加信息)
		stBar->addPermanentWidget(label1); //将文本信息绑定到状态栏中 (从右往左添加信息)
```

### 1.4.6 铆接部件/浮动窗口

```c++
#include <QDockWidget>
	/* 浮动窗口(铆接部件)可以有多个,能从主窗口中拖拽出来[围绕 核心部件/中心部件使用] */
	QDockWidget *dkWidget = new QDockWidget("text", this);
		addDockWidget(Qt::BottomDockWidgetArea, dkWidget); //设置浮动窗口 并设置默认停靠位
		dkWidget->setAllowedAreas(Qt::LeftToolBarArea | Qt::ReightToolBarArea)  //设置浮动窗口允许停靠的位置

```

### 1.4.7 核心部件

```c++
#include <QTextEdit>
/* 中心部件一般只设置一个 */
	QTextEdit *textEdit = new QtextEdit(this); //文本编辑 #include <QTextEdit>
	setCentralWidget(textEdit); //设置为中间内容
```

## 1.5 UI界面

### 1.5.1 资源文件添加

```c++
ui->object->setIcon(QIcon(": + 前缀 + src/a.png")); 	//添加小图标
	ui>object->setIcon(QIcon(:/images/a.png));
```

### 1.5.2 UI界面绑定信号和槽

```c++
# UI控件绑定信号和槽
connect(ui->object, &QAction::triggered, [=](){ });
	connect(ui->pushbotton_1, &QPushButton::clicked, this,[=](){ });

# UI控件
	push Button //按钮
	Tool Button //工具按钮, 一般只用来显示图片
	Radio Button //单选按钮
		ui->object->setChecked(true); //单选框默认选中
	Group Box 		//分组框 (同一类型的单选按钮放在一个分组框里面)
		connct(ui->object, &QCheckBox::stateChanged, [=](int state){ });
					//将选中的状态放入参数 state中,未选中值 选中值为2 半选中为1
```

### 1.5.4 对话框

#### 1.5.4.1 模态对话框和非模态对话框

```c++
/* 模态对话框和非模态对话框 */
#include <QDialog>
	/* 模态对话框(不可以对其他窗口进行编辑) 非模态对话框(可以对其他对话框进行编辑) */
	/* 创建模态对话框 */
		QDialog dl(this);
		dl.resize(500, 350); //指定对话框大小
		dl.exec(); //窗口阻塞
	/* 创建非模态对话框 */
		QDialog* dl = new QDiallog(this);
		dl->resize(120, 40); //指定对话框大小
		dl->show(); //显示对话框
		dl->setAttribute(Qt::WA_DeleteOnClose); //设置对话框属性 (当窗口关闭时new出来的对象同步释放掉)
```

#### 1.5.4.2 标准对话框

```c++
/* 消息对话框 [默认模态对话框]*/
#include <QMessageBox>

	QMessageBox::critical(this, "title", "value"); //错误对话框
		/* 参数: 【父窗口,标题,中间文本内容】*/
	QMessageBox::information(this, "title", "value"); //信息提示对话框
		/* 参数: 【父窗口,标题,中间文本内容】*/
	QMessageBox::warning(this, "title", "value"); //警告对话框
		/* 参数: 【父窗口,标题,中间文本内容】*/
	QMessageBox::question(this, "title", "value", QMessageBox::save | QMessageBox::Cancel,
							QMessageBox::Cancel);
				//询问对话框 返回值按键类型( (父窗口, 标题, 中间文本, 按键类型(默认Yes/No), 默认关联回车的按键)
				/* 参数: 【父窗口,标题,中间文本内容,new几个按钮[按钮类型],关联回车的按键】*/
				/*
					QMessageBox::save // 确定按钮
					QMessageBox::Cancel // 取消按钮
				*/
	if(QMessageBox::Cancel == (QMessageBox::question(this, "title","value",QMessageBox::save |QMessageBox::Cancel))) //判断用户是否点击了第一个按钮
        

/* 颜色选择对话框 */
#include <QColorDialog> //颜色获取对话框,返回用户选择的颜色
	Qcolor a = QColorDialog::getColor(); // 会弹出颜色选择对话框，接收用户选择的颜色属性
	qDeBug<<a.red()<<a.green()<<a.blue();

/* 文件对话框 */
#include <QFileDialog>
	QString fileName = QFileDialog::getOpenFileName(this, "title","./src/", "(*.exe)");
		/*打开文件选择框【父窗口,框标题,默认打开的目录路径,筛选文件后缀名[类型]】, 返回值是选
		择的文件路径 */

/* 字体选择对话框 */
#include <QFontDialog>
	bool ok;
	QFont ft = QFontDialog::getFont(&ok, QFont("微软雅黑", 36))
					//获取用户选取的的字体信息(参数2 为提前设好的默认)
		ft.family(); // 字体 返回值QString类型
		ft.pointSize(); // 字号
		ft.bold(); // 是否加粗
		ft.italic(); // 是否倾斜
```

### 1.5.5 界面布局控件

```c++
" Layouts "
	Vertical Layout 			// 水平布局
	Horizontal Layout 			// 垂直布局
	Grid Layout 				// 网状布局/栅栏布局(两行两列)
	Form Layout 				// 网状布局
```

```c++
/* 单选按钮默认选中*/
	ui->object->setChecked(true); // 按钮默认被选中
	connect(ui->reboot, &QRadioButton::clicked, [](){ }); // 捕获按钮选中信号

/* 列表 */
#include <QListWidgetItem>
listWidget
	QListWidgetItem* item = new QListWidgetItem("text"); //创建的列表行
	item->setTextAlignment(Qt::AlignHCenter); //设置文本水平居中对齐
	ui->object->addItem(item); //将创建的行加入到列表单元中
	ui->object->addItems(QStringList()<<"第一行"<<"第二行"<<"第三行"<<"第四行");
					//一次性将不同行的文本加入列表,但不能设置对齐方式

treeWidget //树列表
	ui->object->setHeaderLabels(QstringList()<<"姓名"<<"性别"); //设置头的标签
	QTreeWidgetItem* liItem = new QTreeWidgetItem(QStringList()<<"第一个根列表"<<"第二个根列表"); // 创建根列表
	ui->object->addTopLevelItem(liItem); //将创建的根列表加载到顶层
		QStringList here1; //添加子节点并设置文本
		here1<<"张三"<<"男";
		QTreeWidgetItem* li1 = new QTreeWidgetItem(here1); //创建子节点变量
		liItem->addChild(li1); //根列表挂载子节点

tableWidget //表格
	ui->object->setColumnCount(3); //设置表格有多少列
	ui->object->setHorizontalHeaderLabels(QStringList()<<"姓名"<<"性别"<<"年龄");  //设置每列的水平表头
	ui->object->setRowCount(5); // 设置行数
	ui->object->setItem(0, 0, new QTableWidgetItem("亚瑟"));  //设置正文(1,2参数为表格的列和行)

		QStringList nameList;
		nameList<<"张三"<<"李四"<<"王二麻子"<<"翠花"<<"二狗子";
		QList<QString> sexList;
		sexList<<"男"<<"男"<<"男"<<"女"<<"男";
		int age[] = {10, 20, 30, 40, 50}
		for(int i=0; i<3; i++)
        {   
			int col =0;
			ui->object->setItem(i, col++, new
			QTableWidgetItem(nameList[i]));
			ui->object->setItem(i, col++, new
			QTableWidgetItem(sexList.at[i]));
			ui->object->setItem(i, col++, new
			QTableWidgetItem(QString::mumber(age)
        }
                             
toolBox //分组栏(QQ好友分组)

tabWidget //分组标签(浏览器上面不同页面之间的)

stackedWidget //栈控件(类似于Qt左侧栏的菜单页)需绑定按钮切换页
	connect(ui->btn, &QPushButton::clicked, [=](){
	ui->stackedWidget->setCurrentIndex(0); //绑定按钮,点击跳转至第0号位页面});
        
frame //做边框用
dockWidget //浮动窗口
comboBox //下拉框
	ui->object->addItem("text");//添加下拉框选项(文本)
fomtComboBox //字体下拉框
lineEdit //单行文本框
textEdit //文本框(可修改里面的字体倾斜颜色)
plainTextEdit //单纯的文本框(不能设置字体倾斜颜色)
spinBox //整数码表(时间戳)
dounleSpinBox //小数码表
timeEdit //时间
dateEdit //日期
date/timeEdit //时间和日期
horizontalSlider //滚动条(视频播放)
label //标签(可加载图片)
	ui->object->setPixmap(QPixmap(": + 前缀 + src/a.png")); //显示图片
	QMovie* img = new QMovie(": + 前缀 + src/a.gif") //创建动态图变量
	ui->object->setMovie(ing); //动态图变量绑定标签
	img->start(); //动态图播放
```

## 1.6 Qt事件

### 1.6.1 鼠标事件

```c++
void enterEvent(QEvent *); //鼠标进入事件
void leaveEvent(QEvent *); //鼠标离开事件
void mousePressEvent(QMouseEvent *ev); //鼠标按下
	QString str = QString("鼠标按下了 x=%1, y=%2").arg(ev->x()).arg(ev->y());
	if(ev->button() == Qt::LeftButton) //判断是否是鼠标左键被按下获取点击位置的信息
void mouseReleaseEvent(QMouseEvent *ev); //鼠标释放
void mouseMoveEvent(QMouseEvent *ev); //鼠标移动
	if(ev->buttons() & Qt::LeftButton) //判断是否为左键按下时移动(连续动作用buttons和位与运算判断不能用 ==)
this->serMouseTracking(true); //设置鼠标追踪(代码写在控件的构造函数里,当鼠标进入控件范围时进行追踪)
```

### 1.6.2 定时器事件

```c++
#定时器事件timerEvent <QTimerEvent>
void timerEvent(QTimerEvent *t); //重构定时器函数
	static int num =1; //构建临时静态变量(临时静态变量只会被初始化一次)
	int time1 = startTimer(1000); //启动定时器 参数单位为毫秒级 返回定时器唯一的ID号
	if(e->timerId() == time1){ }; //当多个不同时间段定时器事件时,判断事件Id执行操作

	QTimer *timer_A = new QTimer(this);//在Widget构造函数中New一个QTimer类 #include<QTimer>
	timer_A->start(500); //启动定时器
	connect(timer_A, &QTimer::timeout, [=](){ qDebug()<<"";});//绑定事件"匿名"函数 timeout每隔指定毫秒抛出信号

	QTime Stop | Continue
	connect(ui->pushButton, &QPushBUtton::clicked, [=](){
	timer_A->stop(); });		//暂停时间事件

```

1.6.3 事件分发器

```c++
#Event事件分发器
	bool Event(Qevent *e); //鼠标分发器,可重写里面的事件捕获函数,在头文件声明源文件实现
	bool MyWidget::event(QEvent *e)
	{
		if(e->type() == QEvent::MouseButtonPress) { return true;}//需要捕获的事件,函数体写返回ture表示事件拦截
		return Widget::event(e); //其他不需要捕获的事件返回给父类处理
	}
```

