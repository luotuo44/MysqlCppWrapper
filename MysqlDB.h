//Author: luotuo44@gmail.com
//Use of this source code is governed by a BSD-style license

#ifndef MYSQLDB_H_
#define MYSQLDB_H_

#include<string>
#include<tuple>
#include<vector>

typedef struct st_mysql MYSQL;


#include"QueryResult.h"




namespace DB
{

template<typename T>
inline std::string valueToString(const T &t)
{
    return std::to_string(t);
}

inline std::string valueToString(const std::string &t)
{
    return "'" + t + "'";
}



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
    QueryResultRowSet query(const std::string &sql);


    //sql likes: insert into student(id, name)
    template<typename ... Args>
    void executeMany(const std::string &sql, std::vector<std::tuple<Args...>> &param);


private:
    template<typename Tuple, size_t N>
    struct TuplePrinter
    {
        static std::string value(const Tuple &tp)
        {
            auto tmp = TuplePrinter<Tuple, N-1>::value(tp);
            return tmp + ", " + valueToString(std::get<N-1>(tp));
        }
    };

    template<typename Tuple>
    struct TuplePrinter<Tuple, 1>
    {
        static std::string value(const Tuple &tp)
        {
            return valueToString(std::get<0>(tp));
        }
    };

private:
    MYSQL *m_conn = nullptr;
    int m_last_error_code = 0;
    std::string m_last_error_msg = "success";
};



template<typename ... Args>
void MysqlDB::executeMany(const std::string &sql, std::vector<std::tuple<Args...>> &param)
{
    if(param.empty())
        return ;

    //insert into student(id, name, score) values('001', 'aa', 81),('002', 'bb', 82),
    //('003', 'cc', 83),('004', 'dd', 84);

    std::string str_param;
    str_param.reserve(param.size() * (sizeof...(Args)) * 10);
    for(const auto &e : param)
    {
        str_param += "(" + TuplePrinter<decltype(e), sizeof...(Args)>::value(e) + "),";
    }

    str_param.pop_back();

    execute(sql + " values" + str_param);
}

}

#endif // MYSQLDB_H_

