# 基于 Qt 待办软件

## 代码架构  

代码分层三层，分别为控制层(controller)、业务逻辑层(service)和数据访问层(dao)  

###  控制层(controller)  

基于Qt事件接受并处理用户请求逻辑，渲染视图或者返回处理后的信息，是整个程序的入口  

### 业务逻辑层(service)  

处理业务逻辑，为数据操作制定规则，这些规则是用于保障数据的有效性、安全性的，使得数据可以随着我们设定的规则而产生或发生变化。通常，在业务层中会使用多个DAO层接口，可以简单理解为业务层中的业务是对一个或者多个dao进行得再次封装，主要是针对具体的问题的操作，把一些数据层的操作进行组合，间接与数据库打交道(提供操作数据库的方法)  

* 例如用户尝试登录时：用户名对应的用户信息必须存在、提交的密码必须与数据库中存储的密码是匹配的  

* 例如用户尝试注册时：在数据库中的用户名不能重复  

* 例如对待办事件的处理：本质上就是对数据库进行增删改查的组合  

  ……
  通过DAO层接口实现的业务层有助于项目的解耦合
### 数据访问层(dao)  

属于一种比较底层基础得操作，具体到对某个表得增删改查，封装了增删改查的方法  

  

## 程序运行流程  

控制层监听用户请求，通过业务逻辑层获得或者修改数据库数据，根据数据进行视图渲染。程序视图大多为树形视图，对树形视图设置了点击事件处理，根据所点击的项去确定具体数据信息，entity如下：  

```c++
class KTask
{
	Q_GADGET
public:
	KTask();
	KTask(int eventId, int userId, const QString& eventName, bool isArranged, int priority,
		const QDateTime& dueDate, bool isRepeated, const QDateTime& remindTime,
		const QString& description, const QString& location, const QString& note);
	KTask(int eventId, int groupId, const QString& eventName, bool isArranged, int priority,
		const QDateTime& dueDate, bool isRepeated, const QDateTime& remindTime,
		const QString& description, const QString& location, const QString& note, bool group);
	~KTask();
	getter();
private:
	int m_id;		// id
	int userId;		// 用户ID
	int groupId;		// 分组ID
	QString eventName;		// 待办名字
	bool arranged;		// 是否安排	
	int priority;		// 优先级
	QDateTime dueDate;		// 截止日期
	bool m_repeated;		// 是否重复
	QDateTime remindTime;		// 提醒日期
	QString description;		// 描述
	QString location;		// 地点
	QString note;		// 备注
};
Q_DECLARE_METATYPE(KTask)		// 注册元对象
```

  注册元对象方便类和QVariant的相互转换    



## 程序重要模块  

### 登录注册模块  

对密码采用MD5加密，为了防止数据库存储的明文密码被攻击者窃取导致密码泄露，通常需要把数据库中存储的密码经过哈希运算后再存储。 而哈希算法单向不可逆的特性，使得即使攻击者获取到哈希值也难以根据哈希值反推出原始密码。

### 首页模块  

```c++
initConnectHome();  // 初始化主页信号和槽函数  

initTaskList(); // 初始化数据容器  

initHomeListWidget(); // 主页视图渲染  

on_treeWidget_itemClicked(QTreeWidgetItem* item, int column); // 树形视图点击处理  
```



### 我的待办模块  

```c++
initTaskList(); // 初始化数据容器

initMyListWidget(); // 我的待办视图渲染

on_treeWidget_itemClicked(QTreeWidgetItem* item, int column); // 树形视图点击处理 
```



### 分组模块  

```c++
initTaskList(); // 初始化数据容器

initGroupWidget(); // 分组视图渲染  

on_treeWidgetGroup_itemClicked(QTreeWidgetItem* item, int column); // 树形视图点击处理
```

  

## 注意事项  

项目数据库选用Mysql，建表文件放于sql目录；项目所连接的数据库部署在云服务器上，可以直接连接。  

默认用户名和密码为admin  

程序将比较耗时的向数据库查询所有待办放于子线程中运行，这样能降低主线程UI的阻塞感，优化使用体验