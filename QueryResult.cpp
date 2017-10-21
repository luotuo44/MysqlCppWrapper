//Author: luotuo44@gmail.com
//Use of this source code is governed by a BSD-style license

#include"QueryResult.h"

#include<assert.h>

#include<stdexcept>


extern "C"
{
#include<mysql.h>
}


namespace DB
{

QueryResultColumn::QueryResultColumn(std::shared_ptr<MYSQL_RES> &res, MYSQL_ROW row, const std::vector<StrInt> &colunm_index)
    : m_res_wp(res),
      m_row(row),
      m_column_name_index(colunm_index)
{
}



size_t QueryResultColumn::columnNum()const
{
    return m_column_name_index.size();
}


std::string QueryResultColumn::columnName(size_t col)const
{
    if(col >= m_column_name_index.size())
        throw std::out_of_range("column " + std::to_string(col) + " is out of range");

    return m_column_name_index[col].first;
}


int QueryResultColumn::columnType(size_t col)const
{
    if(col >= m_column_name_index.size())
        throw std::out_of_range("column " + std::to_string(col) + " is out of range");

    return m_column_name_index[col].second;
}


int QueryResultColumn::getColumnValue(size_t col, int)const
{
    return std::stoi(m_row[col]);
}


long long QueryResultColumn::getColumnValue(size_t col, long long)const
{
    return std::stoll(m_row[col]);
}


double QueryResultColumn::getColumnValue(size_t col, double)const
{
    return std::stod(m_row[col]);
}


std::string QueryResultColumn::getColumnValue(size_t col, const std::string &)const
{
    return m_row[col];
}

}


//===============================================================================

namespace DB
{

QueryResultRowSet::QueryResultRowSet(MYSQL_RES *result_set)
{
    m_result_set = std::shared_ptr<MYSQL_RES>(result_set, [](MYSQL_RES *res){
        mysql_free_result(res);
    });

    fetchField();
    nextRow();
}


QueryResultRowSet::~QueryResultRowSet()
{
}


void QueryResultRowSet::fetchField()
{
    auto num_fields = mysql_num_fields(m_result_set.get());
    m_column_name_index.reserve(num_fields);


    MYSQL_FIELD *field = nullptr;
    while( (field = mysql_fetch_field(m_result_set.get())) )
    {
        m_column_name_index.emplace_back(field->name, field->type);
    }

    assert(num_fields == m_column_name_index.size());
}


void QueryResultRowSet::nextRow()
{
    if(!m_result_set)
        return ;


    ++m_row_index;
    MYSQL_ROW row = mysql_fetch_row(m_result_set.get());
    if(row != nullptr)
        m_cur_row = QueryResultColumn(m_result_set, row, m_column_name_index);
    else
    {
        m_result_set.reset();
        m_cur_row = QueryResultColumn();
        m_row_index = 0;
    }
}


size_t QueryResultRowSet::columnNum()const
{
    return m_column_name_index.size();
}


std::string QueryResultRowSet::columnName(size_t col)const
{
    if(col >= m_column_name_index.size())
        throw std::out_of_range("column " + std::to_string(col) + " is out of range");

    return m_column_name_index[col].first;
}


int QueryResultRowSet::columnType(size_t col)const
{
    if(col >= m_column_name_index.size())
        throw std::out_of_range("column " + std::to_string(col) + " is out of range");

    return m_column_name_index[col].second;
}



}
