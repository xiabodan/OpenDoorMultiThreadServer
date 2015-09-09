<?php
	//打开门
	exec("gpio mode 29 out");/*mode 0就是蓝线插入的针脚，可以使用0-7*/
	usleep(100000);
	exec("gpio write 29 0");/*点亮led*/
	exec("gpio mode 29 in");/*mode 0就是蓝线插入的针脚，可以使用0-7*/
?>