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
* - Class Condition, including all fields in a dbelement such as name, 
*   description and datetime.
*   As a friend class of Query, it can get access of private fields of 
*	Condition.
* - Class Query which is responsible for querying matatext, datetime, 
*	key, and children relationship.
*   it holds a reference to database and a collection of keys. It can be 
*	initialized from a databese or a set of keys. You can reset the key 
*	set by db reference.
*   use show function to display all keys in collection.
*   Query Supports AND and OR query manipulation. It can AND using the
*	query result from another query. It can implement OR from two seperate
*	key sets or from two seperate query instances.
*
*

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


/////////////////////////////////////////////////////////////////////
// Condition class
// - provides fields of dbelement
class Condition
{
public:
	Condition() { dateSelected = false; }

	//set functions
	Condition& description(std::string re);
	Condition& datetime(DateTime date);
	Condition& name(std::string re);
	Condition& key(std::string re);


private:
	template<typename T>
	friend class Query;

	std::string description_re;
	std::string name_re;
	std::string key_re;
	DateTime date_;
	bool dateSelected;
};

/////////////////////////////////////////////////////////////////////
// Query class
// - provides core query operations
// - supports AND and OR query operations
template<typename T>
class Query
{
public:
	using Key = std::string;
	using Keys = std::vector<Key>;

	Query<T>(NoSqlDb::DbCore<T>& db);
	Query& from(const Keys& keys);

	//general query by condtion class
	Query& select(const Condition &c);

	//specific seletion
	Query& selectWithPayLoad(T t);
	Query& selectKey(const std::string &re);
	Query& selectName(const std::string &re);
	Query& selectDescription(const std::string &re);
	Query& selectDate(const DateTime &startTime, const DateTime &endTime = DateTime().now());
	Query& selectChildren(const Key &key);
	Query& selectKeysWithChild(const Key &key);
	
	//union query
	Query& unionFrom(const Keys& keys1, const Keys& keys2);
	Query& unionSelect(const Condition &c1, const Condition &c2);

	Keys& keys() { return keys_; }

	void show(std::ostream& out = std::cout);
	void reset();

private:
	Keys keys_;
	// reference to db
	NoSqlDb::DbCore<T>& db_;
};



/////////////////////////////////////////////////////////////////////
// init methods

template<typename T>
Query<T>::Query(NoSqlDb::DbCore<T>& db):db_(db), keys_(db.keys())
{
}

//----< init key from other key sets >----------------------
template<typename T>
Query<T>& Query<T>::from(const Keys& keys)
{
	keys_ = keys;
	return *this;
}

//----< set key set to the key in dbcore >----------------------
template<typename T>
void Query<T>::reset()
{
	keys_ = db_.keys();
}

/////////////////////////////////////////////////////////////////////
// selection methods

//----< select by condition instance >----------------------
template<typename T>
Query<T>& Query<T>::select(const Condition &c)
{
	selectKey(c.key_re);
	selectName(c.name_re);
	selectDescription(c.description_re);
	if (c.dateSelected) selectDate(c.date_);

	return *this;
}

//template<>
//Query<std::string>& Query<std::string>::selectWithPayLoad(std::string re)
//{
//	if (re.length() == 0) return *this;
//	std::regex e(re);
//	Keys selectRes;
//	for (Key key : keys_) {
//		if (std::regex_match(db_[key].payLoad(), e)) {
//			selectRes.push_back(key);
//		}
//	}
//	keys_ = selectRes;
//	return *this;
//}

//----< select by payload type >----------------------
// Use a payload instance as a select template
// fields to be match are determined by payload itself
// every uesr-defined payload type should implement 
// a compare function
template<typename T>
Query<T> & Query<T>::selectWithPayLoad(T t)
{
	Keys selectRes;
	for (Key key : keys_) {
		if (db_[key].payLoad().compare(t)) {
			selectRes.push_back(key);
		}
	}
	keys_ = selectRes;
	return *this;
}

//----< select by key >----------------------
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

//----< select by name >----------------------
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

//----< select by description >----------------------
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

//----< select by date, endTime is set to now() as default >----------------------
template<typename T>
Query<T>& Query<T>::selectDate(const DateTime &startTime, const DateTime &endTime)
{
	Keys selectRes;
	for (Key key : keys_) {
		if (db_[key].dateTime() > startTime && db_[key].dateTime() < endTime) {
			selectRes.push_back(key);
		}
	}
	keys_ = selectRes;
	return *this;
}

//----< select by children relationship, return children of the key >----------------------
template<typename T>
Query<T>& Query<T>::selectChildren(const Key &key) {
	keys_ = db_[key].children();
	return *this;
}

//----< select by child, return the "parent" keys of the query key >----------
template<typename T>
Query<T> & Query<T>::selectKeysWithChild(const Key & key)
{
	Keys selectRes;
	for (Key key_ : keys_) {
		if (find(db_[key_].children().begin(), db_[key_].children().end(), key) != db_[key_].children().end())
			selectRes.push_back(key_);
	}
	keys_ = selectRes;
	return *this;
}

/////////////////////////////////////////////////////////////////////
// union query methods

//----< OR query from two key sets, key the unique ones >----------------------
template<typename T>
Query<T> & Query<T>::unionFrom(const Keys & keys1, const Keys & keys2)
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

//----< OR query from two conditions>----------------------
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
		out << key;
		out << "  ";
	}
}