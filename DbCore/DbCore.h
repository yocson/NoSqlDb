#pragma once
/////////////////////////////////////////////////////////////////////
// DbCore.h - Implements NoSql database prototype                  //
// ver 2.0														   //
// Cheng Wang													   //	
// cwang76@syr.edu												   //
// Source: Jim Fawcett, CSE687, Object Oriented Design, Spring 2018//
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides two classes:
* - DbCore implements core NoSql database operations, but does not
*   provide editing, querying, or persisting.  Those are left as
*   exercises for students.
* - DbElement provides the value part of our key-value database.
*   It contains fields for name, description, date, child collection
*   and a payload field of the template type. 
* The package also provides functions for displaying:
* - set of all database keys
* - database elements
* - all records in the database

* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* DateTime.h, DateTime.cpp
* Utilities.h, Utilities.cpp
* XmlParser.h, XmlParser.cpp
* XmlDocument.h, XmlDocument.cpp
*
* Maintenance History:
* --------------------
* ver 2.0 : Feb 5, 2018
* - final release
* ver 1.5 : Jan 31, 2018
* - added persistence
* ver 1.4 : Jan 29, 2018
* - added edition operations
* ver 1.3 : Jan 28, 2018
* - added children operation
* ver 1.2 : Jan 26, 2018
* - added addtion and deletion of element
* ver 1.1 : 19 Jan 2018
* - added code to throw exception in index operators if input key is not in database
* ver 1.0 : 10 Jan 2018
* - first release
*/

#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "../DateTime/DateTime.h"
#include "../XmlDocument/XmlParser/XmlParser.h"
#include "../XmlDocument/XmlDocument/XmlDocument.h"

namespace NoSqlDb
{
  /////////////////////////////////////////////////////////////////////
  // DbElement class
  // - provides the value part of a NoSql key-value database

  template<typename T>
  class DbElement
  {
  public:
    using Key = std::string;
    using Children = std::vector<Key>;

    // methods to get and set DbElement fields

    std::string& name() { return name_; }
    std::string name() const { return name_; }
	DbElement& name(const std::string& name) { name_ = name; return *this; }

    std::string& descrip() { return descrip_; }
    std::string descrip() const { return descrip_; }
	DbElement& descrip(const std::string& name) { descrip_ = name; return *this; }
    
    DateTime& dateTime() { return dateTime_; }
    DateTime dateTime() const { return dateTime_; }
	DbElement& dateTime(const DateTime& dateTime) { dateTime_ = dateTime; return *this;}

    Children& children() { return children_; }
    Children children() const { return children_; }
	DbElement& children(const Children& children) { children_ = children; return *this;}

    T& payLoad() { return payLoad_; }
    T payLoad() const { return payLoad_; }
	DbElement& payLoad(const T& payLoad) { payLoad_ = payLoad; return *this; }

  private:
    std::string name_;
    std::string descrip_;
    DateTime dateTime_;
    Children children_;
    T payLoad_;
  };

  /////////////////////////////////////////////////////////////////////
  // DbCore class
  // - provides core NoSql db operations
  // - does not provide editing, querying, or persistance operations

  template <typename T>
  class DbCore
  {
  public:
    using Key = std::string;
    using Keys = std::vector<Key>;
    using Children = Keys;
    using DbStore = std::unordered_map<Key,DbElement<T>>;
    using iterator = typename DbStore::iterator;

    // methods to access database elements

    Keys keys();
    bool contains(const Key& key);
    size_t size();
    void throwOnIndexNotFound(bool doThrow) { doThrow_ = doThrow; }
    DbElement<T>& operator[](const Key& key);
    DbElement<T> operator[](const Key& key) const;
    typename iterator begin() { return dbStore_.begin(); }
    typename iterator end() { return dbStore_.end(); }

    // methods to get and set the private database hash-map storage

    DbStore& dbStore() { return dbStore_; }
    DbStore dbStore() const { return dbStore_; }
    void dbStore(const DbStore& dbStore) { dbStore_ = dbStore; }

	// methods to add and delete key-value pair
	void addElem(const Key& key, DbElement<T> dbElem);
	void addElem(const Key& key, std::string name, std::string descrip, T t, DateTime datetime=DateTime().now());
	void deleteElem(const Key& key);
	
	// methods to edit dbelement
	void editTextMata(const Key& key, std::string type, std::string newText);
	void editDatetime(const Key& key, const DateTime& datetime);
	void addChild(const Key& key, const Key& childKey);
	void deleteChild(const Key& key, const Key& childKey);
	void replaceElem(const Key& key, DbElement<T> dbElem);
	//bool checkChild(const Keys& parentKey, const Key& childKey);

	// methods for persistence
	void ReadFromXML(const std::string& src);
	void SaveToXML(const std::string& src);

	// check children relationship when demostrate the element
	void checkChildren(DbElement<T>& elem);

  private:
    DbStore dbStore_;
    bool doThrow_ = false;
  };

  //----< add element to db with dbelement>----------------------
  template<typename T>
  void DbCore<T>::addElem(const Key& key, DbElement<T> dbElem)
  {
	  if (contains(key))
	  {
		  if (doThrow_)
			  throw(std::exception("key exists in db"));
	  }
	  dbStore_.insert(make_pair(key, dbElem));
  }

  //----< add element to db with specific fields>----------------
  template<typename T>
  inline void DbCore<T>::addElem(const Key & key, std::string name, std::string descrip, T t, DateTime datetime)
  {
	  if (contains(key))
	  {
		  if (doThrow_)
			  throw(std::exception("key exists in db"));
	  }
	  DbElement<T> dbElem;
	  dbElem.name(name);
	  dbElem.descrip(descrip);
	  dbElem.dateTime(datetime);
	  dbElem.payLoad(t);
	  dbStore_.insert(make_pair(key, dbElem));
  }

  //----< edit element textmate with specific type>----------------
  template<typename T>
  void DbCore<T>::editTextMata(const Key & key, std::string type, std::string newText)
  {
	  if (!contains(key))
	  {
		  if (doThrow_)
			  throw(std::exception("key dose not exist in db"));
		  else
			  return;
	  }
	  if (type == "name") {
		  dbStore_[key].name(newText);
	  }
	  else if (type == "descrip") {
		  dbStore_[key].descrip(newText);
	  }
	  else {
		  if (doThrow_)
			  throw(std::exception("invalid text type"));
		  else
			  return;
	  }
  }

  //----< edit element datetime>----------------
  template<typename T>
  inline void DbCore<T>::editDatetime(const Key & key, const DateTime & datetime)
  {
	  if (!contains(key))
	  {
		  if (doThrow_)
			  throw(std::exception("key dose not exist in db"));
		  else
			  return;
	  }
	  dbStore_[key].dateTime(datetime);
  }

  //----< delete element from db>----------------------------------
  template<typename T>
  void DbCore<T>::deleteElem(const Key& key)
  {
	  if (!contains(key))
	  {
		  if (doThrow_)
			  throw(std::exception("key dose not exist in db"));
		  else
			  return;
	  }
	  dbStore_.erase(key);
  }

  //----< add child relationship>----------------------------------
  template<typename T>
  void DbCore<T>::addChild(const Key& key, const Key& childKey)
  {
	  if (!contains(key))
	  {
		  if (doThrow_)
			  throw(std::exception("key dose not exist in db"));
		  else
			  return;
	  }
	  if (!contains(childKey))
	  {
		  if (doThrow_)
			  throw(std::exception("Chile key is invalid"));
		  else
			  return;
	  }
	  dbStore_[key].children().push_back(childKey);
  }

  //----< delete child relationship>----------------------------------
  template<typename T>
  void DbCore<T>::deleteChild(const Key& key, const Key& childKey)
  {
	  if (!contains(key))
	  {
		  if (doThrow_)
			  throw(std::exception("key dose not exist in db"));
		  else
			  return;
	  }
	  std::vector<std::string>::iterator iter = find(dbStore_[key].children().begin(), dbStore_[key].children().end(), childKey);
	  if (iter != dbStore_[key].children().end()) dbStore_[key].children().erase(iter);
  }

  //----< replace exsiting elem with a new elem>----------------------------------
  template<typename T>
  inline void DbCore<T>::replaceElem(const Key & key, DbElement<T> dbElem)
  {
	  if (!contains(key))
	  {
		  if (doThrow_)
			  throw(std::exception("key does not exist in db"));
		  else
			  return;
	  }
	  dbStore_[key] = dbElem;
  }

  //----< Persistence read from a exsiting xml file>-----------
  template<typename T>
  void DbCore<T>::ReadFromXML(const std::string &src)
  {
	  XmlProcessing::XmlParser parser(src);
	  XmlProcessing::XmlDocument* pDoc = parser.buildDocument();
	  std::vector<XmlProcessing::XmlDocument::sPtr> records = pDoc->descendents("dbRecord").select();
	  bool skip = false;
	  std::string trimedStr = "";
;	  for (auto record : records) {
		  DbElement<T> tempElem;
		  Key key;
		  skip = false;
		  std::vector<XmlProcessing::XmlDocument::sPtr> contents = record->children();
		  for (auto content : contents) {
			  if (content->tag() == "key") {
				  if (contains(content->children()[0]->value())) {
					  skip = true;
					  break;
				  }
				  key = content->children()[0]->value();
			  } 
			  if (content->tag() == "value") {
				  for (auto attr : content->attribute()) {
					  if (attr.first == "name")
						  tempElem.name(attr.second);
					  if (attr.first == "description") 
						  tempElem.descrip(attr.second);
					  if (attr.first == "dateTime") 
						  tempElem.dateTime(attr.second);
				  }
				  for (auto value : content->children()) {
					  if (value->tag() == "payLoad")
						  trimedStr = value->children()[0]->toString().substr(3);
						  tempElem.payLoad(trimedStr);
					  if (value->tag() == "children") {
						  Children children;
						  for (auto child : value->children())
							  children.push_back(child->children()[0]->value());
						  tempElem.children(children);
					  }
				  }
			  }
		  }
		  if (skip != true) addElem(key, tempElem);  
	  }
  }

  //----< Persistence save datebase to a exsiting xml file>-----------
  template<typename T>
  void DbCore<T>::SaveToXML(const std::string & src)
  {
	  using Sptr = std::shared_ptr<XmlProcessing::AbstractXmlElement>;
	  Sptr pDb = XmlProcessing::makeTaggedElement("db");
	  Sptr pDocElem = XmlProcessing::makeDocElement(pDb);
	  XmlProcessing::XmlDocument xDoc(pDocElem);

	  for (auto item : dbStore_) {
		  Sptr pRecord = XmlProcessing::makeTaggedElement("dbRecord");
		  pDb->addChild(pRecord);
		  Sptr pKey = XmlProcessing::makeTaggedElement("key", item.first);
		  pRecord->addChild(pKey);

		  Sptr pValue = XmlProcessing::makeTaggedElement("value");
		  Sptr pPayload = XmlProcessing::makeTaggedElement("payLoad", item.second.payLoad());
		  pValue->addChild(pPayload);
		  pValue->addAttrib("name", item.second.name());
		  pValue->addAttrib("description", item.second.descrip());
		  pValue->addAttrib("dateTime", item.second.dateTime());
		  pRecord->addChild(pValue);

		  if (item.second.children().size()) {
			  Sptr pChildren = XmlProcessing::makeTaggedElement("children");
			  for (auto child : item.second.children()) {
				  if (contains(child)) {
					  Sptr pChild = XmlProcessing::makeTaggedElement("childKey", child);
					  pChildren->addChild(pChild);
				  }
				  else {
					  std::vector<Key>::iterator iter = find(item.second.children().begin(), item.second.children().end(), child);
					  item.second.children().erase(iter);
				  }
			  }
			  pValue->addChild(pChildren);
		  }
	  }
	  std::string Xml = xDoc.toString();
	  std::ofstream file(src);
	  if (!file.is_open()) {
		  if (doThrow_)
			  throw(std::exception("fail to open file"));
		  else
			  return;
	  }
	  file << Xml << std::endl;
	  file.close();
  }

  /////////////////////////////////////////////////////////////////////
  // DbCore<T> methods

  //----< does db contain this key? >----------------------------------

  template<typename T>
  bool DbCore<T>::contains(const Key& key)
  {
    iterator iter = dbStore_.find(key);
    return iter != dbStore_.end();
  }
  //----< returns current key set for db >-----------------------------

  template<typename T>
  typename DbCore<T>::Keys DbCore<T>::keys()
  {
    DbCore<T>::Keys dbKeys;
    DbStore& dbs = dbStore();
    size_t size = dbs.size();
    dbKeys.reserve(size);
    for (auto item : dbs)
    {
      dbKeys.push_back(item.first);
    }
    return dbKeys;
  }
  //----< return number of db elements >-------------------------------

  template<typename T>
  size_t DbCore<T>::size()
  {
    return dbStore_.size();
  }
  //----< extracts value from db with key >----------------------------
  /*
  *  - indexes non-const db objects
  *  - In order to create a key-value pair in the database like this:
  *      db[newKey] = newDbElement
  *    we need to index with the new key and assign a default element. 
  *    That will be replaced by newDbElement when we return from operator[]
  *  - However, if we want to index without creating new elements, we need
  *    to throw an exception if the key does not exist in the database.
  *  - The behavior we get is determined by doThrow_.  If false we create
  *    a new element, if true, we throw. Creating new elements is the default
  *    behavior.
  */
  template<typename T>
  DbElement<T>& DbCore<T>::operator[](const Key& key)
  {
    if (!contains(key))
    {
      if (doThrow_)
        throw(std::exception("key does not exist in db"));
      else
        return (dbStore_[key] = DbElement<T>());
    }
    return dbStore_[key];
  }
  //----< extracts value from db with key >----------------------------
  /*
  *  - indexes const db objects
  */
  template<typename T>
  DbElement<T> DbCore<T>::operator[](const Key& key) const
  {
    if (!contains(key))
    {
      throw(std::exception("key does not exist in db"));
    }
    return dbStore_[key];
  }

  //----< check children relationship when demostrate the element >----------------------------
  template<typename T>
  void DbCore<T>::checkChildren(DbElement<T>& elem) {
	  Keys newChildKeys;
	  for (auto child : elem.children()) {
		  if (contains(child)) {
			  newChildKeys.push_back(child);
		  }
	  }
	  elem.children() = newChildKeys;
  }
  
  /////////////////////////////////////////////////////////////////////
  // display functions

  //----< display database key set >-----------------------------------

  template<typename T>
  void showKeys(DbCore<T>& db, std::ostream& out = std::cout)
  {
    out << "\n  ";
    for (auto key : db.keys())
    {
      out << key << " ";
    }
  }
  //----< show record header items >-----------------------------------

  inline void showHeader(std::ostream& out = std::cout)
  {
    out << "\n  ";
    out << std::setw(26) << std::left << "DateTime";
    out << std::setw(15) << std::left << "Name";
    out << std::setw(25) << std::left << "Description";
    out << std::setw(40) << std::left << "Payload";
    out << "\n  ";
    out << std::setw(26) << std::left << "------------------------";
    out << std::setw(15) << std::left << "------------";
    out << std::setw(25) << std::left << "-----------------------";
    out << std::setw(40) << std::left << "-----------------------";
  }
  //----< display DbElements >-----------------------------------------

  template<typename T>
  void showElem(const DbElement<T>& el, std::ostream& out = std::cout)
  {
    out << "\n  ";
    out << std::setw(26) << std::left << std::string(el.dateTime());
    out << std::setw(15) << std::left << el.name();
    out << std::setw(25) << std::left << el.descrip();
    out << std::setw(40) << std::left << el.payLoad();
    typename DbElement<T>::Children children = el.children();
    if (children.size() > 0)
    {
      out << "\n    child keys: ";
      for (auto key : children)
      {
		  out << " " << key;
      }
    }
  }
  //----< display all records in database >----------------------------
  // check children exsitence before show element
  template<typename T>
  void showDb(DbCore<T>& db, std::ostream& out = std::cout)
  {
    showHeader(out);
    typename DbCore<T>::DbStore dbs = db.dbStore();
    for (auto item : dbs)
    {
		db.checkChildren(item.second);
		showElem(item.second, out);
    }
  }
}
