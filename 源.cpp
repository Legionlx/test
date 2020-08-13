#define _CRT_SECURE_NO_DEPRECATE //添加定义忽略 ERROR C4996

#include <stdio.h>
#include <WinSock.h>
#include "include/mysql.h"
#include <Windows.h>

//包含附加依赖项
#pragma comment(lib,"wsock32.lib")
#pragma comment(lib,"libmysql.lib")

MYSQL mysql; //mysql连接
MYSQL_FIELD* fd; //字段列数组
char field[32][32]; //存字段名二维数组
MYSQL_RES* res; //这个结构代表返回行的一个查询结果集
MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列
char query[150]; //查询语句

bool ConnectDatabase();
void FreeConnect();
bool QueryDatabase1();
bool QueryDatabase2();
bool InsertData();
bool ModifyData();
bool DeleteData();

//该程序执行一个循环，即查询当前数据→插入新数据→修改数据→删除数据
int main(int argc, char** argv)
{
    ConnectDatabase();

    QueryDatabase1();

    InsertData();
    QueryDatabase2();

    ModifyData();
    QueryDatabase2();

    DeleteData();
    QueryDatabase2();

    FreeConnect();
    system("pause");
    return 0;
}

bool ConnectDatabase() //连接数据库
{
    mysql_init(&mysql);  //初始化

    if (!(mysql_real_connect(&mysql, "localhost", "root", "991104", "test", 0, NULL, 0)))
        //分别为主机，用户名，密码，数据库名，端口号
    {
        printf("Error connecting to database:%s\n", mysql_error(&mysql));
        return false;
    }
    else
    {
        printf("Connected!\n");
        return true;
    }
}

void FreeConnect() //释放资源
{
    mysql_free_result(res);
    mysql_close(&mysql);
}

bool QueryDatabase1()
{
    sprintf(query, "select * from user"); //执行查询语句
    mysql_query(&mysql, "set names gbk"); //设置编码格式

    if (mysql_query(&mysql, query)) //执行SQL语句，返回0查询成功，返回1查询失败
    {
        printf("Query failed (%s)\n", mysql_error(&mysql));
        return false;
    }
    else
    {
        printf("Query successful\n");
    }

    if (!(res = mysql_store_result(&mysql))) //获得sql语句结束后返回的结果集
    {
        printf("Couldn't get result from %s\n", mysql_error(&mysql));
        return false;
    }

    printf("number of dataline returned: %d\n", mysql_affected_rows(&mysql)); //打印数据行数

    char* str_field[32]; //定义一个字符串数组存储字段信息
    for (int i = 0; i < 4; i++) //在已知字段数量的情况下获取字段名
    {
        str_field[i] = mysql_fetch_field(res)->name;
    }
    for (int i = 0; i < 4; i++) //打印字段
        printf("%10s\t", str_field[i]);
    printf("\n");

    while (column = mysql_fetch_row(res)) //在已知字段数量情况下，获取并打印下一行
    {
        printf("%10s\t%10s\t%10s\t%10s\n", column[0], column[1], column[2], column[3]);
    }
    return true;
}

bool QueryDatabase2()
{
    mysql_query(&mysql, "set names gbk");
    
    if (mysql_query(&mysql, "select * from user"))
    {
        printf("Query failed (%s)\n", mysql_error(&mysql));
        return false;
    }
    else
    {
        printf("Query successful\n");
    }
    res = mysql_store_result(&mysql);
    
    printf("number of dataline returned: %d\n", mysql_affected_rows(&mysql));
    for (int i = 0; fd = mysql_fetch_field(res); i++) //获取字段名
        strcpy(field[i], fd->name);

    int j = mysql_num_fields(res); // 获取列数
    for (int i = 0; i < j; i++)
        printf("%10s\t", field[i]);
    printf("\n");

    while (column = mysql_fetch_row(res))
    {
        for (int i = 0; i < j; i++)
            printf("%10s\t", column[i]);
        printf("\n");
    }
    return true;
}

bool InsertData()
{
    sprintf(query, "insert into user values (6, 'Lilei', '2588752','lilei23@sina.cn');");
    if (mysql_query(&mysql, query))
    {
        printf("Insert failed (%s)\n", mysql_error(&mysql));
        return false;
    }
    else
    {
        printf("Insert successful\n");
        return true;
    }
}

bool ModifyData()
{
    sprintf(query, "update user set email='lilei325@163.com' where name='Lilei'");
    if (mysql_query(&mysql, query))
    {
        printf("Modify failed (%s)\n", mysql_error(&mysql));
        return false;
    }
    else
    {
        printf("Modify successful\n");
        return true;
    }
}

bool DeleteData()
{
    sprintf(query, "delete from user where id=6");

    if (mysql_query(&mysql, query))
    {
        printf("Delete failed (%s)\n", mysql_error(&mysql));
        return false;
    }
    else
    {
        printf("Delete successful\n");
        return true;
    }
}