# MysqlCppWrapper
用C++11 封装mysql的C API，目前支持整数、字符串类型的插入和查询功能，并支持批量批量插入功能。


## 连接数据库以及简单的执行语句

需要先部署mysql数据库服务器，才能连接并执行。
```cpp
std::string host("xxxx");
int port = 3306;
std::string user("root");
std::string pw("12345678");
std::string db("test");

DB::MysqlDB conn(host, port, user, pw, db);
conn.setCharacterSet("utf8");
conn.execute("drop table if exists student");
std::string sql(R"(create table if not exists student(
				id int AUTO_INCREMENT,
				name varchar(32),
				score int,
				PRIMARY KEY(id))charset='utf8')");

conn.execute(sql);
```

## 简单插入和查询语句

```cpp
std::string host("xxxx");
int port = 3306;
std::string user("root");
std::string pw("12345678");
std::string db("test");

DB::MysqlDB conn(host, port, user, pw, db);
conn.setCharacterSet("utf8");//重要

sql = "insert into student(name, score) values('xiaoming', 86)";
conn.execute(sql);
std::cout<<"auto increment id = "<<conn.autoIncrementId()<<std::endl;

sql = "insert into student(name, score) values('张三', 92)";
conn.execute(sql);
std::cout<<"auto increment id = "<<conn.autoIncrementId()<<std::endl;


sql = "select id, name, score from student";
DB::QueryResultRowSet rows = conn.query(sql);
size_t column_index = 0;
std::generate_n(std::ostream_iterator<std::string>(std::cout, "\t"), rows.columnNum(), [&column_index, &rows]{
	return rows.columnName(column_index++);
});
std::cout<<endl<<"------------------------"<<std::endl;


for(const auto &row : rows)
{
	std::cout<<row.getColumn<int>("id")<<"\t"
			<<row.getColumn<std::string>("name")
			<<"\t"<<row.getColumn<int>("score")<<std::endl;
}


```

输出结果为：

>auto increment id = 1
>auto increment id = 2
>id	name	score	
>\------------------------
>1	xiaoming	86
>2	张三			92



## 批量插入和查询语句


```cpp

std::string host("xxxx");
int port = 3306;
std::string user("root");
std::string pw("12345678");
std::string db("test");

DB::MysqlDB conn(host, port, user, pw, db);
conn.setCharacterSet("utf8");
conn.execute("drop table if exists student");
std::string sql(R"(create table if not exists student(
				id int AUTO_INCREMENT,
				name varchar(32),
				score int,
				PRIMARY KEY(id))charset='utf8')");

conn.execute(sql);


sql = "insert into student(name, score)";
std::vector<std::tuple<std::string, int>> vec;
vec.emplace_back("老大", 1000);
vec.emplace_back("老二", 926);
vec.emplace_back("老三", 93);
vec.emplace_back("laod", 90);
conn.executeMany(sql, vec);


sql = "select * from student where score > 90";
DB::QueryResultRowSet rows = conn.query(sql);

size_t column_index = 0;
std::generate_n(std::ostream_iterator<std::string>(std::cout, "\t"), rows.columnNum(), [&column_index, &rows]{
	return  rows.columnName(column_index++);
});


DB::QueryResultRowSet it = rows, end;
DB::QueryResultRowSet tmp = it++;
for(; it != end; ++it)
{
	std::cout<<it->getColumn<int>(0)<<"\t"
			<<it->getColumn<std::string>(1)
			<<"\t"<<it->getColumn<int>(2)<<std::endl;
}


auto first_row = *tmp;
std::cout<<first_row.getColumn<int>("id")
		<<"\t"<<first_row.getColumn<std::string>("name")<<"\t"
		<<first_row.getColumn<int>("score")<<std::endl;


```

输出结果为:

|id|name|score|
|:--:|:--:|:--:|
|2	| 老二 |	926
|3	| 老三 |	93
|1	| 老大 |	1000
