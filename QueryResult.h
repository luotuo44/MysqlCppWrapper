//Author: luotuo44@gmail.com
//Use of this source code is governed by a BSD-style license

#ifndef QUERYRESULT_H_
#define QUERYRESULT_H_


#include<string>
#include<vector>
#include<algorithm>
#include<utility>
#include<stdexcept>
#include<iterator>
#include<memory>


typedef char **MYSQL_ROW;
typedef struct st_mysql_res MYSQL_RES;
typedef struct st_mysql_field MYSQL_FIELD;
using StrInt = std::pair<std::string, int>;


namespace DB
{
class QueryResultColumn
{
public:
    QueryResultColumn()=default;
    QueryResultColumn(std::shared_ptr<MYSQL_RES> &res, MYSQL_ROW row, const std::vector<StrInt> &colunm_index);


public:
    size_t columnNum()const;
    std::string columnName(size_t col)const;
    int columnType(size_t col)const;

    template<typename T>
    T getColumn(size_t col)const;

    template<typename T>
    T getColumn(const std::string &column_name)const;


private:
    int getColumnValue(size_t col, int)const;
    long long getColumnValue(size_t col, long long)const;
    double getColumnValue(size_t col, double)const;
    std::string getColumnValue(size_t col, const std::string &)const;

private:
    std::weak_ptr<MYSQL_RES> m_res_wp;
    MYSQL_ROW m_row = nullptr;
    //存储列的名称以及类型enum_field_types
    std::vector<StrInt> m_column_name_index;
};



template<typename T>
inline T QueryResultColumn::getColumn(size_t col)const
{
    if( col >= m_column_name_index.size() )
        throw std::out_of_range("column " + std::to_string(col) + " is out of range");

    auto sp = m_res_wp.lock();//不能直接使用if(！m_res_wp.lock())测试，要用一个命名变量hold住
    if(!sp)
        throw std::logic_error(" mysql fetch result has free ");

    return getColumnValue(col, T());
}


template<typename T>
inline T QueryResultColumn::getColumn(const std::string &column_name)const
{
    auto it = std::find_if(m_column_name_index.begin(), m_column_name_index.end(), [&column_name](const StrInt &p){
       return column_name == p.first;
    });

    if(it == m_column_name_index.end())
        throw std::out_of_range("cannot find column: " + column_name);

    return getColumn<T>(std::distance(m_column_name_index.begin(), it));
}




//=========================================================================

class QueryResultRowSet : public std::iterator<std::input_iterator_tag, QueryResultColumn>
{
public:
    QueryResultRowSet()=default;
    explicit QueryResultRowSet(MYSQL_RES *result_set);
    ~QueryResultRowSet();

    size_t columnNum()const;
    std::string columnName(size_t col)const;
    int columnType(size_t col)const;

private:
    void fetchField();
    void nextRow();


public:
    using self = QueryResultRowSet;

    reference operator * () { return m_cur_row; }
    pointer operator -> ()  { return &(operator *()); }

    self& operator ++ ()    { nextRow(); return *this; }
    self  operator ++ (int) { self tmp(*this); ++(*this); return tmp; }


    bool operator == (const QueryResultRowSet &rs)const { return m_result_set == rs.m_result_set && m_row_index == rs.m_row_index;}
    bool operator != (const QueryResultRowSet &rs)const { return !(*this == rs); }


    QueryResultRowSet begin() { return *this; }
    QueryResultRowSet end() { return QueryResultRowSet(); }

private:
    std::shared_ptr<MYSQL_RES> m_result_set;
    size_t m_row_index = 0;

    QueryResultColumn m_cur_row;
    //存储列的名称以及类型enum_field_types
    std::vector<StrInt> m_column_name_index;
};

}

#endif // QUERYRESULT_H_

