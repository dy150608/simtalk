#include<iostream>  
#include<string>  
#include <cstring>
#include<mysql.h> 
#include <iomanip>
#include "../tool/for_simple.hpp"

using namespace std;

int main()
{
    //必备数据结构  
    MYSQL mydata;  //=mysql_init((MYSQL*)0); 
    if (NULL != mysql_init(&mydata)) 
        cout << "mysql_init()succeed" << endl;
    else {
        cout << "mysql_init()failed" << endl;
        return -1;
    }

    //初始化数据库  
    if (0 == mysql_library_init(0, NULL, NULL)) 
        cout << "mysql_library_init()succeed" << endl;  
    else {
        cout << "mysql_library_init()failed" << endl;
        return -1;
    }
    //连接数据库   //这里的地址，用户名，密码，数据库，端口可以根据自己本地的情况更改  
    if (NULL != mysql_real_connect(&mydata, "localhost", "root", "xjz150608", "account_manage", 3306, NULL, 0))    
    {
        cout << "mysql_real_connect()succeed" << endl;
    }
    else
    {
        cout << "mysql_real_connect()failed" << endl;
        return -1;
    }

    //MYSQL myCont;
    MYSQL_RES *result;
    MYSQL_ROW sql_row;
    QUERY("select * from account_info");
    int res;
    //mysql_query(&mydata, "SET NAMES GBK"); //设置编码格式
    res = mysql_real_query(&mydata, QUERY_SQL, QUERY_LEN);//查询
    if (!res)
    {
        result = mysql_store_result(&mydata);
    //  result = mysql_use_result(&mydata);//使用这句的时候，必须使用mysql_fetch_row读完全部的行，直到为NULL
        if (result)
        {
            unsigned int num_fields = mysql_num_fields(result);
            cout << "num_fields: " << num_fields << endl;

            MYSQL_FIELD *field;
        //  mysql_field_seek(result, 1);
            while ((field = mysql_fetch_field(result)))
            {
                cout << setiosflags(ios::left)<< setw(15) << field->name;
            }
            cout << endl;
            //mysql_data_seek(result, 2);           
            while (sql_row = mysql_fetch_row(result))//获取具体的数据
            {       
                for (int i = 0; i < mysql_num_fields(result); i++)
                {
                    if (sql_row[i])
                        cout << setiosflags(ios::left)<< setw(15) << sql_row[i];
                //  else cout << "               ";
                }
                cout << endl;           
            }
        }
        mysql_free_result(result);
    }
    else
    {
        cout << "query sql failed!" << endl;
    }

    //操作……  
    mysql_close(&mydata);
    mysql_library_end();
    cout << "end..." << endl;
    return 0;
}
