# OpenDoorMultiThreadServer
OpenDoorMultiThreadServer
实验室门禁打卡系统
1、mydb是操作数据库Mysql类
2、public是一些公用的函数，开门，字符串操作
3、log  是打印日志
4、OpenDoorMultiThreadServer是主服务器，包括多线程服务器，系统初始化

用到的第三方库有：wiringPi  pthread mysqlclient 

数据库包括三张表：config opentime people 分别用于系统初始化配置，打卡时间记录，人员管理
使用方法
         make
         sudo ./OpenDoorMultiThreadServer
