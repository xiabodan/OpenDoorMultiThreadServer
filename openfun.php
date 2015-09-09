
<?php
	//打开门
	//exec("gpio mode 29 out");/*mode 0就是蓝线插入的针脚，可以使用0-7*/
	//usleep(100000);
	//exec("gpio write 29 0");/*点亮led*/
	//exec("gpio mode 29 in");/*mode 0就是蓝线插入的针脚，可以使用0-7*/
?>


<div style="margin:0 auto;width:200px;height:60px;margin:auto;font-size:20pt;color:blue" value="OpenDoor" onclick="window.open('./OpenDoor.php')"> 
<input class="submit" id="btn" type="submit" style="margin:0 auto;width:200px;height:60px;margin:auto;font-size:20pt;color:green" value="OpenDoor" /> 
</div> 


<?php
        $user_IP = ($_SERVER["HTTP_VIA"]) ? $_SERVER["HTTP_X_FORWARDED_FOR"] : $_SERVER["REMOTE_ADDR"];
        $user_IP = ($user_IP) ? $user_IP : $_SERVER["REMOTE_ADDR"];
        echo $user_IP;
		//$time = time();
		//echo date("y-m-d",$time) ;
		
?>

<?php
function getIP() /*获取客户端IP*/
{
        if (@$_SERVER["HTTP_X_FORWARDED_FOR"])
                $ip = $_SERVER["HTTP_X_FORWARDED_FOR"];
        else if (@$_SERVER["HTTP_CLIENT_IP"])
                $ip = $_SERVER["HTTP_CLIENT_IP"];
        else if (@$_SERVER["REMOTE_ADDR"])
                $ip = $_SERVER["REMOTE_ADDR"];
        else if (@getenv("HTTP_X_FORWARDED_FOR"))
                $ip = getenv("HTTP_X_FORWARDED_FOR");
        else if (@getenv("HTTP_CLIENT_IP"))
                $ip = getenv("HTTP_CLIENT_IP");
        else if (@getenv("REMOTE_ADDR"))
                $ip = getenv("REMOTE_ADDR");
        else
                $ip = "Unknown";
        return $ip;
}

	//echo getip();
?>

<?
//显示静态文本，居中
//	echo '<center><p style="font-size:30pt;color:red">Well Come to R1-14  Have a Good Day<p></center>';
?>
<?php 
function InitMysql()
{
	
} 
?>

<?php 
	date_default_timezone_set(Asia/Chongqing );
	//链接数据路
	$server_name="localhost:3306"; //数据库服务器名称 
	$username="root"; // 连接数据库用户名 
	$password="xiabo891219"; // 连接数据库密码 
	$mysql_database="r114"; // 数据库的名字 
	// 连接到数据库 
	$conn=mysql_connect($server_name, $username, $password); 
	mysql_query("set names gbk");
	// 从表中提取信息的sql语句 
	
	//执行sql查询 
	//mysql_select_db($mysql_database,$conn);
	
	//将peopleworkbegin时间读取上来，用于计算当日打卡时间
	$ip = getip(); //获取客户端IP地址
	$sqlpeopleworkbegin = sprintf("select peopleworkbegin from people where peopleipaddress='%s';",$ip);
	$resultpeopleworkbegin=mysql_db_query($mysql_database, $sqlpeopleworkbegin, $conn);  //mysql_query($sql);
	mysql_data_seek($resultpeopleworkbegin, 0); 
	while ($row=mysql_fetch_row($resultpeopleworkbegin))
	{
		for ($i=0; $i<mysql_num_fields($resultpeopleworkbegin); $i++ ) 
		{ 
			$btime = $row[$i];
			//echo $btime;
		}
	}
	//上下班统计次数
	$sqlnum = sprintf("select opentimenum from opentime where opentimeip='%s';",$ip);
	//echo $sqlnum;
	$result=mysql_db_query($mysql_database, $sqlnum, $conn);  //mysql_query($sql);
	mysql_data_seek($result, 0); 
	while ($row=mysql_fetch_row($result)) 
	{ 
		for ($i=0; $i<mysql_num_fields($result); $i++ ) 
		{ 
			//echo $row[$i];
			if($row[$i] == '0') 
			{
				//将当前时间写入peopleworkbegin时间
				$time = time();
				$sql = sprintf("update people  set peopleworkbegin='%s' where peopleipaddress='%s';",date("H:i:s",$time),$ip); //H大写表示24进制时间，小写表示12进制时间
				//echo $sql;
				$result=mysql_db_query($mysql_database, $sql, $conn);  //mysql_query($sql);
				//保存当天打卡次数
				$sql = sprintf("update opentime  set opentimenum=%d where opentimeip='%s';",$row[$i]+1,$ip);
				//echo $sql;
				$result=mysql_db_query($mysql_database, $sql, $conn);  //mysql_query($sql);
			}
			else
			{
				//将当前时间写入peopleworkafter时间
				$time = time();
				//echo $time;
				$atime  = date("H:i:s",$time);
				//echo $atime;
				$sql = sprintf("update people  set peopleworkafter='%s' where peopleipaddress='%s';",date("H:i:s",$time),$ip);
				//echo $sql;
				$result=mysql_db_query($mysql_database, $sql, $conn);  //mysql_query($sql);
				//保存当天打卡次数
				$sql = sprintf("update opentime  set opentimenum=%d where opentimeip='%s';",$row[$i]+1,$ip);
				//echo $sql;
				$result=mysql_db_query($mysql_database, $sql, $conn);  //mysql_query($sql);
				
				
				$ctime = strtotime($atime)-strtotime($btime )-8*60*60;//计算当日打开时间
				//echo $ctime;
				//$ctime =  date("H:i:s",$ctime);
				$sql = sprintf("update people  set peopletodayworktime='%s' where peopleipaddress='%s';",date("H:i:s",$ctime),$ip);
				//echo $sql;
				$result=mysql_db_query($mysql_database, $sql, $conn);  //mysql_query($sql);
			}
		}
	} 
	
	
	
	//查询表中所有的数据并显示表
	$strsql="select * from people "; 
	$result=mysql_db_query($mysql_database, $strsql, $conn); 
	// 获取查询结果 
	// 定位到第一条记录 
	mysql_data_seek($result, 0); 
	// 循环取出记录 
	// ------开始打印表格--------
	//http://blog.csdn.net/21aspnet/article/details/6801206
	//http://www.cnblogs.com/mydomain/archive/2012/11/02/2750542.html
	//echo"<table border=\"1\">";
	echo '<font face="verdana">';     
    echo '<table border="1" cellpadding="1" cellspacing="2">';    
	echo"<tr>";
	echo"<td>ID</td>";
	echo"<td>����</td>";
	echo"<td>�Ա�</td>";
	echo"<td>���</td>";
	echo"<td>����</td>";
	echo"<td>����</td>";
	echo"<td>�ϰ�ʱ��</td>";
	echo"<td>�°�ʱ��</td>";
	echo"<td>�ۼ�ʱ��</td>";
	echo"<td>�涨ʱ��</td>";
	echo"<td>���칤��ʱ��</td>";
	echo"<td>����̶�IP</td>";
	echo"<td>�绰����</td>";
	echo"</tr>";
	while ($row=mysql_fetch_row($result)) 
	{ 
		echo"<tr>";
		for ($i=0; $i<mysql_num_fields($result); $i++ ) 
		{ 
			echo"<td>".$row[$i]."</td>";
			//echo $row[$i];
			//echo "\n\n";	//空两个格符号	
		}
		echo"</tr>";
		//echo "<br/>";	//浏览器换行符，文本换行符还是\n
	} 
	echo"</table>";
	// ------结束打印表格--------
	// 释放资源 
	
	mysql_free_result($result); 
	// 关闭连接 
	mysql_close($conn);  

?> 



<?php
	/*
	$a=13.1;
	echo"<table border=\"1\">";
	echo"<tr>";
	echo"<td>xiabo</td>";
	echo"<td>ceil</td>";
	echo"<td>floor</td>";
	echo"<td>round</td>";
	echo"</tr>";
	echo"<tr>";
	echo"<td>".$a."</td>";
	echo"<td>".ceil($a)."</td>";
	echo"<td>".floor($a)."</td>";
	echo"<td>".round($a)."</td>";
	echo"</tr>";
	echo"<tr>";
	echo"<td>".$a."</td>";
	echo"<td>".ceil($a)."</td>";
	echo"<td>".floor($a)."</td>";
	echo"<td>".round($a)."</td>";
	echo"</tr>";
	echo"</table>";
	*/
?>



<?php
/*
//显示10行12列
echo "<table><tr><td>";
$i = 12;
for ($b=1;$b<=120;$b++){
        if (($b-1)%$i == 0) {
                echo "</td></tr><tr><td>";
        }
        echo "$b\n";
}
echo "</td></tr></table>";
*/
?> 