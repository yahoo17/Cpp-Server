#include "connection_pool.h"
using namespace std

connection_pool::connection_pool * m_instance=nullptr;
connection_pool::connection_pool():Maxconn(0),CurConn(0)
{
	
}
connection_pool::~connection_pool()
{
	DestroyPool();
}
//构造初始化
void connection_pool::init(string url,string User,string Password,string DBname,int port,unsigned int MaxConn)
{
	this->url=url;
	this->User=User;
	this->Password=Password;
	this->DatabaseName=DBname;
	this->Port=port;

	lock.lock();
	for(int i=0li<Maxconn;i++)
	{
		MYSQL * con=NULL;
		con=mysql_init(con);
		if(con==NULL)
		{
			cout<<"Error:"<<mysql_error(con);
			exit(1);
		}
		con=mysql_real_connect(con,url.c_str,User.c_str,Password.c_str,DBname.c_str,Port,NULL,0);
		if(con==NULL)
		{
			cout<<"Error"<<mysql_error(con);
			exit(1);
		}
		

		connlist.push_back(con);
		++FreeConn;
	}
	reserve=sem(FreeConn);
	this->Maxconn=FreeConn;
	lock.unlock();
}
MYSQL * connection_pool::GetConnection()
{
	MYSQL * con=NULL;
	if(connlist.size()<=0)
		return NULL;
	reserve.wait();
	lock.lock();
	con=connlist.front();
	connlist.pop_front();
	--FreeConn;
	++CurConn;
	lock.unlock();
	return con;

}
bool connection_pool::ReleaseConnection(MYSQL * con)
{
	if(NULL==con)
		return false;
	lock.lock();
	connlist.push_back(con);
	++FreeConn;
	--CurConn;
	lock.unlock();
	reserve.post();
	return true;
}
void connection_pool::DestroyPool()
{
	lock.lock();
	if(connlist.size()>0)
	{
		list<MYSQL *>::iterator it;
		for(it=connlist.begin();it!=connlist.end();it++)
		{
			MYSQL  *con=*it;
			mysql_close(con);
		}
		CurConn=0;
		FreeConn=0;
		connlist.clear();
		lock.unlock();
	}
	lock.unlock();
}
int connection_pool::GetFreeConn()
{
	return this->FreeConn;
}

connectionRAII::connectionRAII(MYSQL ** SQL, connection_pool * connpool)
{
	*SQL=connpool->GetConnection();
	connRAII=*SQL;
	poolRAII=connpool;

}
connectionRAII::~connectionRAII()
{
	poolRAII->ReleaseConnection(conRAII);
}