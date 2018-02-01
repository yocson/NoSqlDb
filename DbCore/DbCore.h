#pragma once
/////////////////////////////////////////////////////////////////////
// DbCore.h - Implements NoSql database prototype                  //
// ver 1.0                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018       //
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
*
* Maintenance History:
* --------------------
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
    void name(const std::string& name) { name_ = name; }

    std::string& descrip() { return descrip_; }
    std::string descrip() const { return descrip_; }
    void descrip(const std::string& name) { descrip_ = name; }
    
    DateTime& dateTime() { return dateTime_; }
    DateTime dateTime() const { return dateTime_; }
    void dateTime(const DateTime& dateTime) { dateTime_ = dateTime; }

    Children& children() { return children_; }
    Children children() const { return children_; }
    void children(const Children& children) { children_ = children; }

    T& payLoad() { return payLoad_; }
    T payLoad() const { return payLoad_; }
    void payLoad(const T& payLoad) { payLoad_ = payLoad; }

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
	void editTextMata(const Key& key, std::string type, std::string newText);
	void editDatetime(const Key& key, const DateTime& datetime);
	void deleteElem(const Key& key);
	void addChild(const Key& key, const Key& childKey);
	void deleteChild(const Key& key, const Key& childKey);
	void replaceElem(const Key& key, DbElement<T> dbElem);

	void ReadFromXML(const std::string& src);
	void SaveToXML(const std::string& src);
  
  private:
    DbStore dbStore_;
    bool doThrow_ = false;
  };

  //----< add element to db>----------------------------------
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

  template<typename T>
  inline void DbCore<T>::editTextMata(const Key & key, std::string type, std::string newText)
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

  template<typename T>
  void DbCore<T>::ReadFromXML(const std::string &src)
  {
	  XmlProcessing::XmlParser parser(src);
	  parser.verbose();
	  XmlProcessing::XmlDocument* pDoc = parser.buildDocument();
	  std::vector<XmlProcessing::XmlDocument::sPtr> records = pDoc->descendents("dbRecord").select();
	  for (auto record : records) {
		  DbElement<T> tempElem;
		  Key key;
		  std::vector<std::shared_ptr<XmlProcessing::AbstractXmlElement>> contents = record->children();
		  for (auto content : contents) {
			  if (content->tag() == "key") {
				  key = content->children()[0]->value();
			  } 
			  if (content->tag() == "value") {
				  for (auto attr : content->attribute()) {
					  if (attr.first == "name") {
						  tempElem.name(attr.second);
					  }
					  if (attr.first == "description") {
						  tempElem.descrip(attr.second);
					  }
					  if (attr.first == "dateTime") {
						  tempElem.dateTime(attr.second);
					  }
				  }
				  tempElem.payLoad(content->children()[0]->value());
			  }
		  }
		  addElem(key, tempElem);
	  }
  }

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

		  Sptr pValue = XmlProcessing::makeTaggedElement("value", item.second.payLoad());
		  pValue->addAttrib("name", item.second.name());
		  pValue->addAttrib("description", item.second.name());
		  pValue->addAttrib("dateTime", item.second.dateTime());
		  pRecord->addChild(pValue);
	  }
	  std::string Xml = xDoc.toString();
	  std::cout << Xml << std::endl;
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
    out << std::setw(10) << std::left << "Name";
    out << std::setw(25) << std::left << "Description";
    out << std::setw(25) << std::left << "Payload";
    out << "\n  ";
    out << std::setw(26) << std::left << "------------------------";
    out << std::setw(10) << std::left << "--------";
    out << std::setw(25) << std::left << "-----------------------";
    out << std::setw(25) << std::left << "-----------------------";
  }
  //----< display DbElements >-----------------------------------------

  template<typename T>
  void showElem(const DbElement<T>& el, std::ostream& out = std::cout)
  {
    out << "\n  ";
    out << std::setw(26) << std::left << std::string(el.dateTime());
    out << std::setw(10) << std::left << el.name();
    out << std::setw(25) << std::left << el.descrip();
    out << std::setw(25) << std::left << el.payLoad();
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

  template<typename T>
  void showDb(const DbCore<T>& db, std::ostream& out = std::cout)
  {
    showHeader(out);
    typename DbCore<T>::DbStore dbs = db.dbStore();
    for (auto item : dbs)
    {
      showElem(item.second, out);
    }
  }
}
