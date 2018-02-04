#include <iostream>
#include <iomanip>
#include <functional>
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include "../DbCore/DbCore.h"
#include "../Query/Query.h"
#include "Test.h"
#include "../DbCore/FileInfo.h"

using namespace NoSqlDb;
using namespace TEST;

auto putLine = [](size_t n = 1, std::ostream& out = std::cout)
{
	Utilities::putline(n, out);
};

NoSqlDb::DbCore<FileInfo> Test::testDb;
NoSqlDb::DbCore<std::string> Test::db;

///////////////////////////////////////////////////////////////////////
// DbProvider class
// - provides mechanism to share a test database between test functions
//   without explicitly passing as a function argument.

//class DbProvider
//{
//public:
//	DbCore<std::string>& db() { return db_; }
//private:
//	static DbCore<std::string> db_;
//};
//
//DbCore<std::string> DbProvider::db_;

///////////////////////////////////////////////////////////////////////
// test functions

//----< demo requirement #1 >------------------------------------------

bool Test::testR1()
{
	Utilities::title("Demonstrating Requirement #1");
	std::cout << "\n  " << typeid(std::function<bool()>).name()
		<< ", declared in this function, "
		<< "\n  is only valid for C++11 and later versions.";
	putLine();
	return true; // would not compile unless C++11
}

//----< demo requirement #2 >------------------------------------------

bool Test::testR2()
{
	Utilities::title("Demonstrating Requirement #2");
	std::cout << "\n  A visual examination of all the submitted code "
		<< "will show only\n  use of streams and operators new and delete.";
	putLine();
	return true;
}

//----< demo first part of requirement #3 >----------------------------

bool Test::testR3a()
{
	Utilities::title("Demonstrating Requirement #3a - creating DbElement");
	std::cout << "\n  Creating a db element with key \"Fawcett\":";

	// create database to hold std::string payload

	//DbCore<std::string> db;
	//DbProvider dbp;
	//dbp.db() = db;

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
	putLine();
	return true;
}
//----< demo second part of requirement #3 >---------------------------

bool Test::testR3b()
{
	Utilities::title("Demonstrating Requirement #3b - creating DbCore");

	//DbProvider dbp;
	//DbCore<std::string> db = dbp.db();

	DbElement<std::string> demoElem;

	demoElem.name("Ammar");
	demoElem.descrip("TA for CSE687");
	demoElem.payLoad("You should try ...");
	db["Salman"] = demoElem;
	if (!db.contains("Salman"))
		return false;

	demoElem.name("Jianan");
	demoElem.payLoad("Dr. Fawcett said ...");
	db["Sun"] = demoElem;


	demoElem.payLoad("You didn't demonstrate Requirement #5");
	demoElem.name("Pranjul");
	db["Arora"] = demoElem;

	if (db.size() != 4)
		return false;

	std::cout << "\n  after adding elements with keys: ";
	DbCore<std::string>::Keys keys = db.keys();
	showKeys(db);
	putLine();

	std::cout << "\n  make all the new elements children of element with key \"Fawcett\"";
	db["Fawcett"].children().push_back("Salman");
	db["Fawcett"].children().push_back("Sun");
	db["Fawcett"].children().push_back("Arora");

	showHeader();
	showElem(db["Fawcett"]);

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

bool Test::testR4a()
{
	Utilities::title("Demonstrating Requirement #4a - add pairs");


	DbElement<std::string> demoElem;
	demoElem.name("Calbee");
	demoElem.descrip("You know what");
	demoElem.payLoad("111111111");
	db.addElem("xxx", demoElem);
	if (!db.contains("xxx"))
		return false;

	db.addElem("yyy", "Lemon tea", "yyy", "asdfasdf");
	if (!db.contains("yyy"))
		return false;
	putLine();
	std::cout << "\n  showing all the database elements:";
	std::cout << "\n  two elems added:";
	showDb(db);
	putLine();

	return true;

}

bool Test::testR4b()
{
	Utilities::title("Demonstrating Requirement #4b - delete pairs");

	db.deleteElem("Fawcett");

	if (db.contains("Fawcett")) return false;

	std::cout << "\n  showing all the database elements:";
	std::cout << "\n  Fawcett deleted:";
	showDb(db);
	putLine();
	return true;
}

bool Test::testR5()
{
	Utilities::title("Demonstrating Requirement #5 - edit");

	db.editTextMata("yyy", "name", "Lemon Tee New");
	db.editTextMata("yyy", "descrip", "New Description");
	putLine();
	std::cout << "\n  showing all the database elements:";
	showDb(db);
	putLine();

	std::cout << "\n  add child to yyy";
	db.addChild("yyy", "xxx");
	showElem(db["yyy"]);
	std::cout << "\n  delete child from yyy";
	db.deleteChild("yyy", "xxx");
	showElem(db["yyy"]);
	putLine();

	DbElement<std::string> demoElem2;
	demoElem2.name("vita tee");
	demoElem2.descrip("Pineapple tea");
	demoElem2.payLoad("give me a chance");
	db.replaceElem("yyy", demoElem2);
	putLine();
	std::cout << "\n  showing all the database elements:";
	std::cout << "\n  yyy replaced";
	showDb(db);
	putLine();

	return true;
}

bool Test::testR6()
{
	Utilities::title("Demonstrating Requirement #6 - query");

	db.addElem("zzz", "bottel", "nothing to say", "HIHIHIHI");

	Query<std::string> q1(db);
	std::cout << "\n  showing selected key:" << std::endl;
	q1.selectKey("yyy").show();
	Condition c1;
	c1.name("Calbee");
	q1.reset();
	std::cout << "\n  showing selected name:" << std::endl;
	q1.select(c1).show();
	Condition c2;
	c2.name("vita tee");
	std::cout << "\n  showing selected descrip:" << std::endl;
	Condition c3;
	c3.description("nothing to say");
	Query<std::string> q3(db);

	std::cout << "\n  union select, show return" << std::endl;
	q3.unionSelect(c1, c3).show();

	Query<std::string> q4(db);
	std::cout << "\n  query children, show return" << std::endl;
	db.addChild("yyy", "zzz");
	q4.selectKeysWithChild("zzz").show();




	return true;
}

bool TEST::Test::testR7()
{
	Utilities::title("Demonstrating Requirement #7 - query");

	Condition c1;
	c1.name("Calbee");
	Condition c2;
	c2.description("nothing to say");
	Query<std::string> q1(db);
	q1.select(c1);
	Query<std::string> q2(db);
	q2.select(c2);
	std::cout << "\n  union select, show return" << std::endl;
	Query<std::string> q4(db);
	q4.unionFrom(q1.keys(), q2.keys()).show();


	return true;
}

bool Test::testR8()
{
	Utilities::title("Demonstrating Requirement #8 - xml");


	testDb.ReadFromXML("../test.xml");
	showDb(testDb);
	showKeys(testDb);

	testDb.SaveToXML("../test2.xml");
	testDb.ReadFromXML("../test2.xml");
	showDb(testDb);
	showKeys(testDb);

	return true;
}

bool TEST::Test::testR9()
{
	Utilities::title("Demonstrating Requirement #9 - query payload");
	FileInfo f;
	f.category().insert("CA");
	Query<FileInfo> q1(testDb);
	Utilities::putline(1);
	q1.selectWithPayLoad(f).show();
	
	return true;
}
