#include "mydb.h"
#include "log.h"

CMyDB::CMyDB()
{   
    //初始化连接数据库变量
    connection = mysql_init(NULL);
    if(connection == NULL)
    {
        perror("mysql_init");
        exit(1);
    }
}
CMyDB::~CMyDB()
{   
    //关闭初始化连接数据库变量
    if(connection != NULL)
    {
        mysql_close(connection);
    }
}
//初始化数据库 数据库连接
bool CMyDB::initDB(string server_host , string user, string password , string db_name )
{   
    //运用mysql_real_connect函数实现数据库的连接
    connection_r = mysql_real_connect(connection , server_host.c_str() , user.c_str() , password.c_str() , db_name.c_str() , 0 , NULL , 0);
    if(connection_r == NULL)
    {
		printf("initDB connect error\n");
        perror("mysql_real_connect");
        exit(1);
    }
	else
	{
		printf("initDB connect success\n");
	}
    return true;
}

//执行SQL语句
char*** CMyDB::executeSQL(string sql_str,int* numRows,  int* columnSizes)
{
	
	const char* c_s = sql_str.c_str();
	Fprint_LogString(c_s,LOG_OPENDOOR_FILE,"a+");

    // 查询编码设定
	char*** r;
    if(mysql_query(connection, "set names utf8")){
        fprintf(stderr, "%d: %s\n",mysql_errno(connection), mysql_error(connection));
    }
    int t = mysql_query(connection,  sql_str.c_str());
    if(t){
        printf("Error making query: %s\n" , mysql_error(connection));
        exit(1);
    }else{   
        /*res = mysql_use_result(connection); //初始化逐行的结果集检索
        if(res)
        {
            //mysql_field_count(connection)   返回作用在连接上的最近查询的列数
            for(int i = 0 ; i < mysql_field_count(connection) ; i++)
            {   
                //检索一个结果集合的下一行
                row = mysql_fetch_row(res);   		
                if(row <= 0)
                {
                    break;
                }
                //mysql_num_fields(res)  函数返回结果集中字段的数
                for(int r = 0 ; r  < mysql_num_fields(res) ; r++)
                {
                    printf("%s\t" , row[r]);    
                }
                printf("\n");
            }
        }
        mysql_free_result(res);//释放结果集使用的内存*/
		
		//----------------------------------------------
		res=mysql_store_result(connection);
		if(res) //注意：一定要判断，否则在修改表后会出错，因为修改并没有获取结果
		{
			int f1,f2,num_row, num_col;
			num_row = mysql_num_rows(res); /* Get the no. of row */
			num_col = mysql_num_fields(res); /* Get the no. of column */
			//printf("num_row = %d,num_col = %d\n",num_row,num_col);  //num_col 指的是元素（一个小方格）个数，不是字符个数
			r = (char***)malloc(sizeof(char**) * num_row);
			*numRows = num_row;
			*columnSizes = num_col;
			for (f1 = 0; f1 < num_row; f1++) {
				row = mysql_fetch_row(res); /* Fetch one by one */
				r[f1] = (char**)malloc(sizeof(char*) * num_col);
				for (f2 = 0; f2 < num_col; f2++) {
					printf("%s\t",  row[f2]);
					r[f1][f2] = (char*)malloc(sizeof(char) * (strlen(row[f2])+1));
					strcpy(r[f1][f2],row[f2]);
					//printf("%s\t",  r[f1][f2]); //这句话和printf("%s\t",  row[f2]);一样的
				}
				printf("\n");
			}	
		}
		mysql_free_result(res);
		//mysql_close(sock);
    }
    return r;
}
//表的创建
bool CMyDB::create_table(string table_str_sql)
{
    int t = mysql_query(connection , table_str_sql.c_str());
    if(t)
    {
        printf("Error making query: %s\n" , mysql_error(connection));
        exit(1);
    }
    return true;
}