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

// declare static members
NoSqlDb::DbCore<FileInfo> Test::testDb;
NoSqlDb::DbCore<std::string> Test::db;


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

	DbElement<std::string> demoElem;

	demoElem.name("CPP Language");
	demoElem.descrip("Bjarne Stroustrup");
	demoElem.dateTime(DateTime().now());
	demoElem.payLoad("Addison-Wesley Professional");

	if (demoElem.name() != "CPP Language")
		return false;
	if (demoElem.descrip() != "Bjarne Stroustrup")
		return false;
	if (demoElem.dateTime().now() != DateTime().now())
		return false;
	if (demoElem.payLoad() != "Addison-Wesley Professional")
		return false;

	showHeader();
	showElem(demoElem);

	db["CPPL"] = demoElem;
	putLine();
	return true;
}
//----< demo second part of requirement #3 >---------------------------

bool Test::testR3b()
{
	Utilities::title("Demonstrating Requirement #3b - creating DbCore");

	DbElement<std::string> demoElem;

	demoElem.name("Modern CPP");
	demoElem.descrip("Scott Meyers");
	demoElem.payLoad("O'Reilly Media");
	db["EMCPP"] = demoElem;
	if (!db.contains("EMCPP"))
		return false;

	demoElem.name("SICP");
	demoElem.descrip("Harold Abelson");
	demoElem.payLoad("The MIT Press");
	db["SICP"] = demoElem;


	demoElem.payLoad("Addison-Wesley Professional");
	demoElem.descrip("Stanley B. Lippman");
	demoElem.name("CPP Primer");
	db["CPPP"] = demoElem;

	if (db.size() != 4)
		return false;

	std::cout << "\n  after adding elements with keys: ";
	DbCore<std::string>::Keys keys = db.keys();
	showKeys(db);
	putLine();

	std::cout << "\n  make all the new elements children of element with key \"CPPL\"";
	db["CPPL"].children().push_back("CPPP");
	db["CPPL"].children().push_back("EMCPP");


	showHeader();
	showElem(db["CPPL"]);

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
	demoElem.name("Comp Arch");
	demoElem.descrip("John L. Hennessy");
	demoElem.payLoad("Morgan Kaufmann");
	db.addElem("CA", demoElem);
	if (!db.contains("CA"))
		return false;

	db.addElem("TC", "A Tour of CPP", "Bjarne Stroustrup", "Addison-Wesley");
	if (!db.contains("TC"))
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

	db.deleteElem("CA");

	if (db.contains("CA")) return false;

	std::cout << "\n  showing all the database elements:";
	std::cout << "\n  Fawcett deleted:";
	showDb(db);
	putLine();
	return true;
}

bool Test::testR5()
{
	Utilities::title("Demonstrating Requirement #5 - edit");

	db.editTextMata("TC", "name", "A Tour of CPP 2");
	db.editTextMata("TC", "descrip", "Bjarne Stroustrup 2");
	putLine();
	std::cout << "\n  showing all the database elements:";
	showDb(db);
	putLine();

	std::cout << "\n  add child to yyy";
	db.addChild("CPPL", "TC");
	showElem(db["CPPL"]);
	std::cout << "\n  delete child from CPPL";
	db.deleteChild("CPPL", "EMCPP");
	showElem(db["CPPL"]);
	putLine();

	DbElement<std::string> demoElem2;
	demoElem2.name("CPP Templates");
	demoElem2.descrip("David Vandevoorde");
	demoElem2.payLoad("Addison-Wesley Professional");
	db.replaceElem("TC", demoElem2);
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

	db.addElem("OCPP", "Optimized CPP", "Kurt Guntheroth", "O'Reilly Media");

	Query<std::string> q1(db);
	std::cout << "\n  showing selected key:" << std::endl;
	q1.selectKey("OCPP").show();
	Condition c1;
	c1.name("SICP");
	q1.reset();
	std::cout << "\n  showing selected name:" << std::endl;
	q1.select(c1).show();
	Condition c2;
	c2.name("Modern CPP");
	std::cout << "\n  showing selected descrip:" << std::endl;
	Condition c3;
	c3.description("Kurt Guntheroth");
	Query<std::string> q3(db);

	std::cout << "\n  union select, show return" << std::endl;
	q3.unionSelect(c1, c3).show();

	Query<std::string> q4(db);
	std::cout << "\n  query children, show return" << std::endl;
	q4.selectKeysWithChild("EMCPP").show();

	return true;
}

bool TEST::Test::testR7()
{
	Utilities::title("Demonstrating Requirement #7 - query");

	Condition c1;
	c1.name(".*CPP.*");
	Condition c2;
	c2.description(".*Programming.*");
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
