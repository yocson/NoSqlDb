#pragma once

#include<string>
#include<vector>
#include"DbCore.h"

class Condition
{
public:
	Condition(){}

	Condition& author(std::string re);
	Condition& description(std::string re);
	Condition& datetime(std::string re);
	Condition& name(std::string re);
private:
	friend class Query;
	std::string author_re;
	std::string description_re;
	std::string datetime_re;
	std::string name_re;
};


class Query
{
public:
	using Key = std::string;
	using Keys = std::vector<Key>;

	template<typename T>
	Query(NoSqlDb::DbCore<T> db);


	template<typename T>
	Query& select(T t);

	Query & select(const Condition &c);
	Query& from(const Keys& key);
	Keys& keys();
	void show();

private:
	Keys keys_;
};

template<typename T>
Query::Query(const NoSqlDb::DbCore<T> db)
{
	keys_ = db.keys();
}

Query & Query::select(const Condition &c)
{

}
Query& Query::from(const Keys& keys)
{
	keys_ = keys;
}
Query::Keys& Query::keys()
{
	return keys_;
}
void Query::show()
{

}