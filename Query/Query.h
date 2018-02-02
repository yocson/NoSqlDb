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
#include <set>
#include "../DbCore/DbCore.h"
#include "../DateTime/DateTime.h"

class Condition
{
public:
	Condition() { dateSelected = false; }

	Condition& description(std::string re);
	Condition& datetime(DateTime date);
	Condition& name(std::string re);
	Condition& key(std::string re);
	Condition operator+(const Condition& conds) const;


private:
	template<typename T>
	friend class Query;

	std::string description_re;
	std::string name_re;
	std::string key_re;
	DateTime date_;
	bool dateSelected;
};


template<typename T>
class Query
{
public:
	using Key = std::string;
	using Keys = std::vector<Key>;

	Query<T>(NoSqlDb::DbCore<T>& db);
	Query& from(const Keys& keys);

	Query& select(T t);

	Query& select(const Condition &c);
	Query& exactKey(const Key &key);
	Query& selectKey(const std::string &re);
	Query& selectName(const std::string &re);
	Query& selectDescription(const std::string &re);
	Query& selectDate(const DateTime &startTime, const DateTime &endTime = DateTime().now());
	Query& selectChildren(const Key &key) const;
	
	Query& unionFrom(const Keys& keys1, const Keys& keys2);
	Query& unionSelect(const Condition &c1, const Condition &c2);

	Keys& keys() { return keys_; }

	void show(std::ostream& out = std::cout);
	void reset();

private:
	Keys keys_;
	NoSqlDb::DbCore<T>& db_;
};



/////////////////////////////////////////////////////////////////////
// init methods

template<typename T>
Query<T>::Query(NoSqlDb::DbCore<T>& db):db_(db), keys_(db.keys())
{
}

template<typename T>
Query<T>& Query<T>::from(const Keys& keys)
{
	keys_ = keys;
}

template<typename T>
void Query<T>::reset()
{
	keys_ = db_.keys();
}

/////////////////////////////////////////////////////////////////////
// selection methods

template<typename T>
Query<T>& Query<T>::select(const Condition &c)
{
	selectKey(c.key_re);
	selectName(c.name_re);
	selectDescription(c.description_re);
	if (c.dateSelected) selectDate(c.date_);

	return *this;
}

template<typename T>
inline Query<T> & Query<T>::exactKey(const Key & key)
{
	if (db_.contains(key)) {
		Keys selectRes;
		selectRes.push_back(key);
		Keys_ = selectRes;
	}
	return *this;
}

template<typename T>
Query<T>& Query<T>::selectKey(const std::string &re)
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
Query<T>& Query<T>::selectName(const std::string &re)
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
Query<T>& Query<T>::selectDescription(const std::string &re)
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
Query<T>& Query<T>::selectDate(const DateTime &startTime, const DateTime &endTime)
{
	Keys selectRes;
	for (Key key : keys_) {
		if (db_[key].dateTime() < startTime && db_[key].dateTime() < endTime) {
			selectRes.push_back(key);
		}
	}
	keys_ = selectRes;
	return *this;
}


template<typename T>
Query<T>& Query<T>::selectChildren(const Key &key) const {
	keys_ =  db_[key].children();
	return *this;
}

template<typename T>
inline Query<T> & Query<T>::unionFrom(const Keys & keys1, const Keys & keys2)
{
	std::set<Key> unionSet;
	for (auto key : keys1) {
		unionSet.insert(key);
	}
	for (auto key : keys2) {
		unionSet.insert(key);
	}
	Keys unionKeys;
	for (auto it = unionSet.begin(); it != unionSet.end(); it++) {
		unionKeys.push_back(*it);
	}
	keys_ = unionKeys;
	return *this;
}

template<typename T>
inline Query<T> & Query<T>::unionSelect(const Condition & c1, const Condition & c2)
{
	Keys tempKeys;
	select(c1);
	tempKeys = keys_;
	reset();
	select(c2);
	unionFrom(keys_, tempKeys);
	return *this;
}

/////////////////////////////////////////////////////////////////////
// display methods

template<typename T>
void Query<T>::show(std::ostream& out)
{
	for (Key key : keys_) {
		out << key << std::endl;
	}
}