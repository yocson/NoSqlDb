#pragma once
/////////////////////////////////////////////////////////////////////
// PayLoad.h - Implements user-defined Payload Class               //
// ver 1.0                                                         //
// Cheng Wang, CSE687 - Object Oriented Design, Spring 2018        //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides payload base class and a use-defined FileInfo
* class dereived from base payload class:
* - Base class Payload is an abstract class. It asks every derived calss
*	to implement two functions, fromXML and toXML to interact with XML file
*	, and compare function to realize the general querybyPayload function.
*	toString is essential too because we want to use it in the cout. Also 
*	we want to take the payload as a second parameter of makeTaggedElement,
*	so the cast to string function must be implemented by the derived class.
* - FileInfo class is the class defined in requirement 9, derived from
*	payload class. It has a filepath string and a vector of categories.
*	FileInfo implemented all functions from Payload class so it can be
*	manipulated in dbcore regardless of its inner structures.
*

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
* ver 1.3 :Feb 5, 2018
* - added cast operator to string
* ver 1.2 :Feb 4, 2018
* - added abstract payload class
* ver 1.1 : Feb 3, 2018
* - overload <<
* ver 1.0 : Feb 2, 2018
* - first release
*/
#include <string>
#include <vector>
#include <iostream>
#include <unordered_set>
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlParser/XmlParser.h"

namespace PAYLOAD {
	class PayLoad
	{
	public:
		using Sptr = std::shared_ptr<XmlProcessing::AbstractXmlElement>;
		// deal with XML format
		virtual void fromXML(Sptr payloadTag) = 0;
		virtual Sptr toXML() = 0;
		// used in querywithpayload function
		virtual bool compare(const PayLoad & p) = 0;
		// for cout overloading and for compatibility with saveToXML function
		virtual std::string toString() const = 0;
		virtual operator std::string() = 0;
	};


	class FileInfo : public PayLoad
	{
	public:
		using FilePath = std::string;
		using Category = std::unordered_set<std::string>;
		using Sptr = std::shared_ptr<XmlProcessing::AbstractXmlElement>;

		FileInfo() {}
		FileInfo(std::shared_ptr<XmlProcessing::AbstractXmlElement> payloadTag);
		FileInfo(std::string str);

		void fromXML(Sptr payloadTag);
		Sptr toXML();

		std::string toString() const;
		operator std::string();
		// for showDb
		friend std::ostream& operator<<(std::ostream& out, const FileInfo& f);

		bool compare(const PayLoad & p);

		FilePath& filePath() { return filePath_; }
		FilePath filePath() const { return filePath_; }
		void filePath(const FilePath& fp) { filePath_ = fp; }

		Category& category() { return category_; }
		Category category() const { return category_; }
		void category(const Category& cate) { category_ = cate; }
		void addCate(const std::string &cat);

	private:
		FilePath filePath_;
		Category category_;
		// for compare, it match the filepath and category respectively
		bool matchFilePath(const FilePath &fp);
		bool matchCategory(const Category &cate);
	};
}
