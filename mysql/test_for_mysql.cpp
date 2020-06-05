#include  <stdio.h>
#include <mysql/mysql.h>
const char * db_host="localhost";
const char * db_user="root";
const char * db_pass="123456";
const char * db_name="mydatabase";
const int db_port=3306;
using namespace std;
int main( )
{
    MYSQL * mysql=mysql_init(NULL);
    if(mysql==NULL)
    {
        printf("error when mysql_init");
    }
    mysql =mysql_real_connect(NULL,db_host,db_user,db_pass,db_name,db_port,NULL,0);
    if(mysql)
        printf("success!");
    else
    {
        
        printf("no success");
    }
    
    
}
