#ifndef  CONNECTION_POOL_H
#define CONNECTION_POOL_h
#include<stdio.h>
#include "../lock/locker.h"
#include<string>
#include <list>
#include <iostream>
using namespace  std;
class connection_pool
{
private:
	mysql * mysqlarray;
public:
	MYSQL * GetConnection();
	bool ReleaseConnection(MYSQL * conn);
	int GetFreeConn();
	void DestroyPool();

	connection_pool();
	~connection_pool();

public:
	static connection_pool * m_instance;
	static connection_pool * getInstance()
	{
		if(m_instance==nullptr)
			m_instance = new connection_pool();
		return m_instance;
	}
	void init(string url,string User,string PassWord,string DataBaseName,int port,unsigned int Maxconn);

private:
	unsigned int Maxconn;	//max

	unsigned int CurConn;	// used

	unsigned int FreeConn;// free

private:
	locker lock;
	list<MYSQL *> connlist;	//连接池
	sem reserve;

private:
	string url;
	string 	Port;
	string User;
	string Password;
	string  DatabaseName;
	

};

class connectionRAII
{
public:
	connectionRAII(MYSQL ** con,connection_pool * connpool);
	~connectionRAII( )
private:
	MYSQL *connRAII ;
	connection_pool * poolRAII;

};
#endif