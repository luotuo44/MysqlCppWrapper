//Author: luotuo44@gmail.com
//Use of this source code is governed by a BSD-style license

#include<iostream>

#include"MysqlDB.h"

using namespace std;


std::string host;
std::string user;
std::string pw;
std::string db;
int port;


void testBaseExecute()
{
    DB::MysqlDB conn(host, port, user, pw, db);
    conn.setCharacterSet("utf8");
    conn.execute("drop table if exists student");
    std::string sql(R"(create table if not exists student(
                    id int AUTO_INCREMENT,
                    name varchar(32),
                    score int,
                    PRIMARY KEY(id))charset='utf8')");

    conn.execute(sql);

    sql = "insert into student(name, score) values('xiaoming', 86)";
    conn.execute(sql);
    std::cout<<"auto increment id = "<<conn.autoIncrementId()<<std::endl;

    sql = "insert into student(name, score) values('张三', 92)";
    conn.execute(sql);
    std::cout<<"auto increment id = "<<conn.autoIncrementId()<<std::endl;
}


int main(int argc, char **argv)
{
	if( argc < 6 )
	{
		std::cout<<"usage: "<<argv[0]<<" host port user password database "<<std::endl;
		return -1;
	}

	host = argv[1];
	port = std::stoi(argv[2]);
	user = argv[3];
	pw = argv[4];
	db = argv[5];

    testBaseExecute();

    cout << "Hello World!" << endl;
    return 0;
}

