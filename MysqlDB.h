//Author: luotuo44@gmail.com
//Use of this source code is governed by a BSD-style license

#ifndef MYSQLDB_H_
#define MYSQLDB_H_

#include<string>

typedef struct st_mysql MYSQL;



namespace DB
{


class MysqlDB
{
public:
    MysqlDB()=default;
    MysqlDB(const std::string &host, int port, const std::string &user, const std::string &password, const std::string &db = "information_schema");

    MysqlDB(const MysqlDB&)=delete;
    MysqlDB& operator = (const MysqlDB&)=delete;
    ~MysqlDB();

    void connect(const std::string &host, int port, const std::string &user, const std::string &password, const std::string &db = "information_schema");
    int lastErrorCode()const { return m_last_error_code; }
    std::string lastErrorMsg()const { return m_last_error_msg; }


private:
    void initAndConnect(const std::string &host, int port, const std::string &user, const std::string &password, const std::string &db);
    void checkIntReturnValue(const std::string &msg, int ret);
    void checkPointerReturnValue(const std::string &msg, void *p);

public:
    void setCharacterSet(const std::string &char_set);
    void execute(const std::string &sql);
    long long autoIncrementId();



private:
    MYSQL *m_conn = nullptr;
    int m_last_error_code = 0;
    std::string m_last_error_msg = "success";
};


}

#endif // MYSQLDB_H_

