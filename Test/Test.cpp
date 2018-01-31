#include <iostream>
#include <iomanip>
#include <functional>
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include "../DbCore/DbCore.h"
#include "../Query/Query.h"
#include "Test.h"

using namespace NoSqlDb;

auto putLine = [](size_t n = 1, std::ostream& out = std::cout)
{
	Utilities::putline(n, out);
};

///////////////////////////////////////////////////////////////////////
// DbProvider class
// - provides mechanism to share a test database between test functions
//   without explicitly passing as a function argument.

class DbProvider
{
public:
	DbCore<std::string>& db() { return db_; }
private:
	static DbCore<std::string> db_;
};

DbCore<std::string> DbProvider::db_;

///////////////////////////////////////////////////////////////////////
// test functions

//----< demo requirement #1 >------------------------------------------

bool TEST::testR1()
{
	Utilities::title("Demonstrating Requirement #1");
	std::cout << "\n  " << typeid(std::function<bool()>).name()
		<< ", declared in this function, "
		<< "\n  is only valid for C++11 and later versions.";
	putLine();
	return true; // would not compile unless C++11
}

//----< demo requirement #2 >------------------------------------------

bool TEST::testR2()
{
	Utilities::title("Demonstrating Requirement #2");
	std::cout << "\n  A visual examination of all the submitted code "
		<< "will show only\n  use of streams and operators new and delete.";
	putLine();
	return true;
}

//----< demo first part of requirement #3 >----------------------------

bool TEST::testR3a()
{
	Utilities::title("Demonstrating Requirement #3a - creating DbElement");
	std::cout << "\n  Creating a db element with key \"Fawcett\":";

	// create database to hold std::string payload

	DbCore<std::string> db;
	DbProvider dbp;
	dbp.db() = db;

	// create some demo elements and insert into db

	DbElement<std::string> demoElem;

	demoElem.name("Jim");
	demoElem.descrip("Instructor for CSE687");
	demoElem.dateTime(DateTime().now());
	demoElem.payLoad("The good news is ...");

	if (demoElem.name() != "Jim")
		return false;
	if (demoElem.descrip() != "Instructor for CSE687")
		return false;
	if (demoElem.dateTime().now() != DateTime().now())
		return false;
	if (demoElem.payLoad() != "The good news is ...")
		return false;

	showHeader();
	showElem(demoElem);

	db["Fawcett"] = demoElem;
	dbp.db() = db;
	putLine();
	return true;
}
//----< demo second part of requirement #3 >---------------------------

bool TEST::testR3b()
{
	Utilities::title("Demonstrating Requirement #3b - creating DbCore");

	DbProvider dbp;
	DbCore<std::string> db = dbp.db();

	DbElement<std::string> demoElem = db["Fawcett"];

	demoElem.name("Ammar");
	demoElem.descrip("TA for CSE687");
	demoElem.payLoad("You should try ...");
	db["Salman"] = demoElem;
	if (!db.contains("Salman"))
		return false;

	demoElem.name("Jianan");
	demoElem.payLoad("Dr. Fawcett said ...");
	db["Sun"] = demoElem;

	demoElem.payLoad("You didn't demonstrate Requirement #4");
	demoElem.name("Nikhil");
	db["Prashar"] = demoElem;

	demoElem.payLoad("You didn't demonstrate Requirement #5");
	demoElem.name("Pranjul");
	db["Arora"] = demoElem;

	demoElem.payLoad("You didn't demonstrate Requirement #6");
	demoElem.name("Akash");
	db["Anjanappa"] = demoElem;

	if (db.size() != 6)
		return false;

	std::cout << "\n  after adding elements with keys: ";
	DbCore<std::string>::Keys keys = db.keys();
	showKeys(db);
	putLine();

	std::cout << "\n  make all the new elements children of element with key \"Fawcett\"";
	db["Fawcett"].children().push_back("Salman");
	db["Fawcett"].children().push_back("Sun");
	db["Fawcett"].children().push_back("Prashar");
	db["Fawcett"].children().push_back("Arora");
	db["Fawcett"].children().push_back("Anjanappa");

	showHeader();
	showElem(db["Fawcett"]);

	db["Salman"].children().push_back("Sun");
	db["Salman"].children().push_back("Prashar");
	db["Salman"].children().push_back("Arora");
	db["Salman"].children().push_back("Anjanappa");

	// display the results

	putLine();
	std::cout << "\n  showing all the database elements:";
	showDb(db);
	putLine();

	std::cout << "\n  database keys are: ";
	showKeys(db);

	putLine();
	return true;
}

bool TEST::testR4()
{
	Utilities::title("Demonstrating Requirement #4 - add and delete pairs");

	DbProvider dbp;
	DbCore<std::string> db = dbp.db();


	DbElement<std::string> demoElem = db["Fawcett"];
	demoElem.name("Ammar");
	demoElem.descrip("TA for CSE687");
	demoElem.payLoad("You should try ...");
	db.addElem("xxx", demoElem);
	if (!db.contains("xxx"))
		return false;

	db.addElem("yyy", "cool", "xxx", "asdfasdf");
	if (!db.contains("yyy"))
		return false;
	putLine();
	std::cout << "\n  showing all the database elements:";
	showDb(db);
	putLine();


	return true;

}

bool TEST::testR5()
{
	Utilities::title("Demonstrating Requirement #5 - edit");

	DbProvider dbp;
	DbCore<std::string> db = dbp.db();

	DbElement<std::string> demoElem = db["Fawcett"];

	demoElem.name("Ammar");
	demoElem.descrip("TA for CSE687");
	demoElem.payLoad("You should try ...");
	db.addElem("xxx", demoElem);
	if (!db.contains("xxx"))
		return false;

	db.addElem("yyy", "cool", "xxx", "asdfasdf");
	if (!db.contains("yyy"))
		return false;

	db.editTextMata("yyy", "name", "newName");
	db.editTextMata("yyy", "descrip", "lallalala");
	//db.deleteElem("Fawcett");
	putLine();
	std::cout << "\n  showing all the database elements:";
	showDb(db);
	putLine();

	db.addChild("Fawcett", "yyy");
	showElem(db["Fawcett"]);
	db.deleteChild("Fawcett", "yyy");
	showElem(db["Fawcett"]);
	putLine();

	DbElement<std::string> demoElem2 = db["Fawcett"];
	demoElem2.name("hhhh");
	demoElem2.descrip("TA for CSE687");
	demoElem2.payLoad("You should not try ...");
	db.replaceElem("Fawcett", demoElem2);
	putLine();
	std::cout << "\n  showing all the database elements:";
	showDb(db);
	putLine();

	return true;
}

bool TEST::testR6()
{
	Utilities::title("Demonstrating Requirement #6 - query");

	DbProvider dbp;
	DbCore<std::string> db = dbp.db();

	db.addElem("aa", "nameaaa", "descriptionaaa", "zxcv");
	db.addElem("bb", "namebbb", "desbb", "qwerty");
	putLine();
	std::cout << "\n  showing all the database elements:";
	showDb(db);
	putLine();
	Query<std::string> q1(db);
	std::cout << "\n  showing selected:";
	q1.selectKey("Fawcett").show();
	Condition c1;
	c1.name("nameaaa");
	q1.reset();
	std::cout << "\n  showing selected:";
	q1.select(c1).show();
	Condition c2;
	c2.name("namebbb");
	std::cout << "\n  showing selected:";
	Condition c3;
	c3.description("Instructor for CSE687");
	//q1.reset();
	//q1.select(c1 + c2 + c3).show();
	Query<std::string> q3(db);
	std::cout << "\n  union select, show return aa, fawcett";
	q3.unionSelect(c1, c3).show();

	return true;
}
