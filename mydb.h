#ifndef __MY_DB_H
 
#include <mysql/mysql.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
using namespace std;
class CMyDB
{
public:
    CMyDB();
    bool initDB(string server_host , string user, string password, string db_name);
    char*** executeSQL(string sql_str,int* numRows,  int* columnSizes);
    bool create_table(string table_str_sql);
	
    ~CMyDB();
public:
	
private:
    MYSQL *connection;
	MYSQL *connection_r;
    MYSQL_RES *res;
    MYSQL_ROW row;
};
#endif