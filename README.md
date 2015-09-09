# OpenDoorMultiThreadServer
OpenDoorMultiThreadServer
实验室门禁打卡系统
1、mydb是操作数据库Mysql类，表示每个人员身份的唯一标准是提前分配的不同的局域网IP
2、public是一些公用的函数，开门，字符串操作
3、log  是打印日志
4、OpenDoorMultiThreadServer是主服务器，包括多线程服务器，系统初始化

用到的第三方库有：wiringPi  pthread mysqlclient 

数据库包括三张表：config opentime people 分别用于系统初始化配置，打卡时间记录，人员管理

另外增加了网页打开开门客户端：采用nginx+mysql+php的模式， OpenDoor.php是打开门的程序，调用树莓派的php接口；openfun.php数据库操作模块，nginx的根目录在/usr/share/nginx/www/ 下；aytorun.sh是开机自启动脚本，在 /etc/rc.local 文件中写bash /home/pi/work/AutoDoor/autorun.sh

使用方法
         make
         sudo ./OpenDoorMultiThreadServer
