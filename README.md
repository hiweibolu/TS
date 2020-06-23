# **火车票系统开发文档**

## **模块划分图**

## **信息存储模块**

用于将各种信息保存至外存的模块。

用于保存信息的文件结构有两种，一种是基于B+树的索引结构，另一种是比较朴素的有序列表。

### **b+树**

用于快速检索符合条件的信息，key值与value值一一对应。

支持的操作有：

 - **单点插入**：将一对key值和value值插入b+树中；如果这个key值已经存在，那么就将原value值替换为新value值。

 - **单点查询**：查询指定的key值对应的value值，如果不存在这个key值，则返回0。

 - **区间查询**：查询指定的key值区间的value值，记录在一个数组中。

 - **清空**。

相关类：<a href='#node'>node</a>, <a href='#btree'>btree</a>.

### **列表**

顺序存储信息，本身一般不具备索引。配合b+树使用，通过b+树索引在列表中的位置，实现存储大块的数据。

支持的操作有：

 - **尾端插入**：将任意类型的数据插入至尾端，返回索引。

 - **取值**：通过索引，在指定位置获取任意类型的数据。

 - **修改**：通过索引，在指定位置写入覆盖任意类型的数据。

 - **清空**。

相关类：<a href='#mylist'>mylist</a>.


## **命令输入模块**

包含多个系统。

 - 对于输入的命令字符串，分解成可供程序识别的解释系统。

 - 将输入的日期或时间转化成以分钟为单位的整型，或者将整型输出成日期+时间的格式的时间系统。

 - 计算字符串的哈希值的哈希系统。

### **解释系统**

对于输入的命令字符串，分解成可供程序识别。

相关类：<a href='#explainer'>explainer</a>.

### **时间系统**

将输入的日期或时间转化成以分钟为单位的整型，或者将整型输出成日期+时间的格式。

相关函数：<a href='#date2day'>date2day</a>, <a href='#date2minute'>date2minute</a>, <a href='#printTime'>printTime</a>.

### **哈希系统**

计算字符串的哈希值。

采用双哈希算法。

相关函数：<a href='#myHash'>myHash</a>.


## **用户管理模块**

管理用户信息，管理用户登录状态。

实现添加用户(add_user)，登录(login)，登出(logout)，查询用户信息(query_profile)，修改用户信息(modify_profile)命令。

使用列表：
 - "users.txt" 以类型user来存储用户的数据。

使用b+树：
 - "userb.txt" 以用户username的哈希值为key值，value值为用户在"users.txt"存储数据的索引。
 - "loginb.txt" 以用户username的哈希值为key值，value值为用户是否登录，1为是，0为否。

相关类：<a href='#user'>user</a>.

相关函数：<a href='#add_user'>add_user</a>, <a href='#login'>login</a>, <a href='#logout'>logout</a>, <a href='#query_profile'>query_profile</a>, <a href='#modify_profile'>modify_profile</a>.

## **车次管理模块**

管理车次信息，并能对其有一定的查询功能。

实现添加车次(add_train)，发布车次(release_train)，查询车次信息(query_train)，删除(delete_train)，查询车票(query_ticket)，查询换乘(query_transfer)命令。

使用列表：
 - "trains.txt" 以类型train来存储车次的数据。
 - "ttrains.txt" 以类型train来存储某天车次的数据。

使用b+树：
 - "trainb.txt" 以车次trainID的哈希值为key值，value值为车次在"trains.txt"存储数据的索引。
 - "ttrainb.txt" 以车次日期+车次trainID的哈希值为组合key值，value值为这天车次在"ttrains.txt"存储数据的索引。
 - "stationb.txt" 以车站名字的哈希值为key值，value值为经过此车站的车次trainID哈希值。

相关类：<a href='#train'>train</a>.

相关函数：<a href='#add_train'>add_train</a>, <a href='#release_train'>release_train</a>, <a href='#query_train'>query_train</a>, <a href='#delete_train'>delete_train</a>, <a href='#query_ticket'>query_ticket</a>, <a href='#query_transfer'>query_transfer</a>.


## **订单管理模块**

管理订单信息，实现候补购票，退票功能，并且在退票后检测并尝试完成候补订单。

实现购票(buy_ticket)，查询用户订单(query_order)，退票(refund_ticket)命令。

使用列表：
 - "orders.txt" 以类型order来存储订单的数据。

相关类：<a href='#order'>order</a>.

相关函数：<a href='#buy_ticket'>buy_ticket</a>, <a href='#query_order'>query_order</a>, <a href='#refund_ticket'>refund_ticket</a>.


## **Reference**

### **约定**


```
typedef unsigned long word;
typedef unsigned long long dword;
```


### **类**

#### <a name='node'>**node**</a>

##### 成员变量

##### 成员函数

#### <a name='btree'>**btree**</a>

#### <a name='mylist'>**mylist**</a>

#### <a name='explainer'>**explainer**</a>

#### <a name='user'>**user**</a>

#### <a name='train'>**train**</a>

#### <a name='order'>**order**</a>

### **函数**
#### <a name='date2day'>**date2day**</a>

```
word date2day(const char* s);
```

将形如"xx-xx"的字符串日期，转化为无符号长整型的天数。

具体数值为1月1日到这个日期所经过的天数乘。


#### <a name='date2minute'>**date2minute**</a>

```
word date2minute(const char* s);
```

将形如"xx-xx"的字符串日期，转化为无符号长整型的分钟数。

具体数值为1月1日到这个日期所经过的天数乘以每天的分钟数。

#### <a name='printTime'>**printTime**</a>

```
void printTime(word x);
```

将无符号长整型的分钟数，输出为"xx-xx xx:xx"格式的日期+时间。

#### <a name='myHash'>**myHash**</a>

```
word myHash(const char *str);
```

通过简单的哈希算法，计算字符串的无符号长整型哈希值。

为了防止冲突，在计算时将长整型划分为两个短整型分别同时计算。

最后将两个短整型再合并成长整型。

#### <a name='add_user'>**add_user**</a>

#### <a name='login'>**login**</a>

#### <a name='logout'>**logout**</a>

#### <a name='query_profile'>**query_profile**</a>

#### <a name='modify_profile'>**modify_profile**</a>

#### <a name='add_train'>**add_train**</a>

#### <a name='release_train'>**release_train**</a>

#### <a name='query_train'>**query_train**</a>

#### <a name='delete_train'>**delete_train**</a>

#### <a name='query_ticket'>**query_ticket**</a>

#### <a name='query_transfer'>**query_transfer**</a>

#### <a name='query_order'>**query_order**</a>

#### <a name='refund_ticket'>**refund_ticket**</a>
