#pragma once

/////////////////////////////////////////////////////////////////////
// Query.h - Implements database query                             //
// ver 1.0                                                         //
// Cheng Wang, CSE687 - Object Oriented Design, Spring 2018        //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides two classes:
* -
*
*
* -
*
*
* The package also provides functions for displaying:
* -
* -
* -

* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* DateTime.h, DateTime.cpp
* Utilities.h, Utilities.cpp
*
* Maintenance History:
* --------------------
* ver 1.1 : 19 Jan 2018
* - added code to throw exception in index operators if input key is not in database
* ver 1.0 : 10 Jan 2018
* - first release
*/

#include <string>
#include <iostream>
#include <vector>
#include <regex>
#include "../DbCore/DbCore.h"

class Condition
{
public:
	Condition() { dateSelected = false; }

	Condition& author(std::string re);
	Condition& description(std::string re);
	Condition& datetime(DateTime date);
	Condition& name(std::string re);
	Condition& key(std::string re);

private:
	friend class Query;
	std::string author_re;
	std::string description_re;
	std::string name_re;
	std::string key_re;
	DateTime date_;
	bool dateSelected;
};


class Query
{
public:
	using Key = std::string;
	using Keys = std::vector<Key>;

	template<typename T>
	Query(NoSqlDb::DbCore<T>& db);
	Query& from(const Keys& key);

	Query& select(const Condition &c);
	Query& selectKey(const std::string &re);
	Query& selectName(const std::string &re);
	Query& selectAuthor(const std::string &re);
	Query& selectDescription(const std::string &re);
	Query& selectDate(const DateTime &startTime);
	Query& selectChildren(const Key &key) const;

	Keys& keys() { return keys_; }

	void show(std::ostream& out = std::cout);
	void reset();

private:
	Keys keys_;
	template<typename T>
	NoSqlDb::DbCore<T>& db_;
};


/////////////////////////////////////////////////////////////////////
// init methods

template<typename T>
Query::Query(const NoSqlDb::DbCore<T>& db)
{
	keys_ = db.keys();
	db_ = db;
}

Query& Query::from(const Keys& keys)
{
	keys_ = keys;
}

template<typename T>
void Query::reset()
{
	keys_ = db_.keys();
}

/////////////////////////////////////////////////////////////////////
// selection methods

Query& Query::select(const Condition &c)
{
	selectKey(c.key_re);
	selectName(c.name_re);
	selectAuthor(c.author_re);
	selectDescription(c.description_re);
	if (c.dateSelected) selectDate(c.datetime);
	return *this;
}

Query& Query::selectKey(const std::string &re)
{
	if (re.length() == 0) return *this;
	std::regex e(re);
	Keys selectRes;
	for (Key key : keys_) {
		if (std::regex_match(key, e)) {
			selectRes.push_back(key);
		}
	}
	keys_ = selectRes;
	return *this;
}

template<typename T>
Query& Query::selectName(const std::string &re)
{
	if (re.length() == 0) return *this;
	std::regex e(re);
	Keys selectRes;
	for (Key key : keys_) {
		if (std::regex_match(db_[key].name(), e)) {
			selectRes.push_back(key);
		}
	}
	keys_ = selectRes;
	return *this;
}

template<typename T>
Query& Query::selectAuthor(const std::string &re)
{
	if (re.length() == 0) return *this;
	std::regex e(re);
	Keys selectRes;
	for (Key key : keys_) {
		if (std::regex_match(db_[key].author(), e)) {
			selectRes.push_back(key);
		}
	}
	keys_ = selectRes;
	return *this;
}

template<typename T>
Query& Query::selectDescription(const std::string &re)
{
	if (re.length() == 0) return *this;
	std::regex e(re);
	Keys selectRes;
	for (Key key : keys_) {
		if (std::regex_match(db_[key].descrip(), e)) {
			selectRes.push_back(key);
		}
	}
	keys_ = selectRes;
	return *this;
}

template<typename T>
Query& Query::selectDate(const DateTime &startTime)
{
	Keys selectRes;
	for (Key key : keys_) {
		if (startTime <= db_[key].dateTime()) {
			selectRes.push_back(key);
		}
	}
	keys_ = selectRes;
	return *this;
}


template<typename T>
Query& Query::selectChildren(const Key &key) const {
	keys_ =  db_[key].children();
	return *this;
}

/////////////////////////////////////////////////////////////////////
// display methods


void Query::show(std::ostream& out = std::cout)
{
	for (Key key : keys_) {
		std::cout << key << std::endl;
	}
}