//Author: luotuo44@gmail.com
//Use of this source code is governed by a BSD-style license


#include"MysqlDB.h"

#include<stdexcept>


extern "C"
{
#include<mysql.h>
}



namespace DB
{

MysqlDB::MysqlDB(const std::string &host, int port, const std::string &user, const std::string &password, const std::string &db)
{
    initAndConnect(host, port, user, password, db);
}


MysqlDB::~MysqlDB()
{
    if(m_conn != nullptr)
        mysql_close(m_conn);
}


void MysqlDB::connect(const std::string &host, int port, const std::string &user, const std::string &password, const std::string &db)
{
    if(m_conn != nullptr)
        throw std::logic_error("has connect to mysql server ago");


    initAndConnect(host, port, user, password, db);
}


void MysqlDB::initAndConnect(const std::string &host, int port, const std::string &user, const std::string &password, const std::string &db)
{
    m_conn = mysql_init(nullptr);
    if(m_conn == nullptr)
        throw std::logic_error("fail to init mysql handler");


    auto ret = mysql_real_connect(m_conn, host.c_str(), user.c_str(), password.c_str(), db.c_str(), port, nullptr, 0);
    if(ret == nullptr)
    {
        auto reason = "fail to connect " + host + ". Reason: " + mysql_error(m_conn);
        mysql_close(m_conn);
        m_conn = nullptr;
        throw std::logic_error(reason);
    }
}



void MysqlDB::checkIntReturnValue(const std::string &msg, int ret)
{
    if( ret != 0 )
    {
        m_last_error_code = ret;
        m_last_error_msg = mysql_error(m_conn);
        auto reason = msg + ". Reason: " + m_last_error_msg;
        throw std::logic_error(reason);
    }
}


void MysqlDB::checkPointerReturnValue(const std::string &msg, void *p)
{
    if( p == nullptr )
    {
        m_last_error_msg = mysql_error(m_conn);
        auto reason = msg + ". Reason: " + m_last_error_msg;
        throw std::logic_error(reason);
    }
}


void MysqlDB::setCharacterSet(const std::string &char_set)
{
    auto ret = mysql_set_character_set(m_conn, char_set.c_str());
    checkIntReturnValue("fail to set CharacterSet for " + char_set, ret);
}


void MysqlDB::execute(const std::string &sql)
{
    auto ret = mysql_query(m_conn, sql.c_str());
    checkIntReturnValue("fail to execute " + sql, ret);
}


long long MysqlDB::autoIncrementId()
{
    return mysql_insert_id(m_conn);
}

}

