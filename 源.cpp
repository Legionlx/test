#define _CRT_SECURE_NO_DEPRECATE //��Ӷ������ ERROR C4996

#include <stdio.h>
#include <WinSock.h>
#include "include/mysql.h"
#include <Windows.h>

//��������������
#pragma comment(lib,"wsock32.lib")
#pragma comment(lib,"libmysql.lib")

MYSQL mysql; //mysql����
MYSQL_FIELD* fd; //�ֶ�������
char field[32][32]; //���ֶ�����ά����
MYSQL_RES* res; //����ṹ�������е�һ����ѯ�����
MYSQL_ROW column; //һ�������ݵ����Ͱ�ȫ(type-safe)�ı�ʾ����ʾ�����е���
char query[150]; //��ѯ���

bool ConnectDatabase();
void FreeConnect();
bool QueryDatabase1();
bool QueryDatabase2();
bool InsertData();
bool ModifyData();
bool DeleteData();

//�ó���ִ��һ��ѭ��������ѯ��ǰ���ݡ����������ݡ��޸����ݡ�ɾ������
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

bool ConnectDatabase() //�������ݿ�
{
    mysql_init(&mysql);  //��ʼ��

    if (!(mysql_real_connect(&mysql, "localhost", "root", "991104", "test", 0, NULL, 0)))
        //�ֱ�Ϊ�������û��������룬���ݿ������˿ں�
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

void FreeConnect() //�ͷ���Դ
{
    mysql_free_result(res);
    mysql_close(&mysql);
}

bool QueryDatabase1()
{
    sprintf(query, "select * from user"); //ִ�в�ѯ���
    mysql_query(&mysql, "set names gbk"); //���ñ����ʽ

    if (mysql_query(&mysql, query)) //ִ��SQL��䣬����0��ѯ�ɹ�������1��ѯʧ��
    {
        printf("Query failed (%s)\n", mysql_error(&mysql));
        return false;
    }
    else
    {
        printf("Query successful\n");
    }

    if (!(res = mysql_store_result(&mysql))) //���sql�������󷵻صĽ����
    {
        printf("Couldn't get result from %s\n", mysql_error(&mysql));
        return false;
    }

    printf("number of dataline returned: %d\n", mysql_affected_rows(&mysql)); //��ӡ��������

    char* str_field[32]; //����һ���ַ�������洢�ֶ���Ϣ
    for (int i = 0; i < 4; i++) //����֪�ֶ�����������»�ȡ�ֶ���
    {
        str_field[i] = mysql_fetch_field(res)->name;
    }
    for (int i = 0; i < 4; i++) //��ӡ�ֶ�
        printf("%10s\t", str_field[i]);
    printf("\n");

    while (column = mysql_fetch_row(res)) //����֪�ֶ���������£���ȡ����ӡ��һ��
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
    for (int i = 0; fd = mysql_fetch_field(res); i++) //��ȡ�ֶ���
        strcpy(field[i], fd->name);

    int j = mysql_num_fields(res); // ��ȡ����
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