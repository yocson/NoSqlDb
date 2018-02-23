#include <iostream>
#include <iomanip>
#include <functional>
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../DbCore/DbCore.h"
#include "../Query/Query.h"
#include "Test.h"
#include "../PayLoad/PayLoad.h"

using namespace NoSqlDb;
using namespace TEST;

auto putLine = [](size_t n = 1, std::ostream& out = std::cout)
{
	Utilities::putline(n, out);
};

// declare static members
NoSqlDb::DbCore<PAYLOAD::FileInfo> Test::fileDb;
NoSqlDb::DbCore<std::string> Test::strDb;


///////////////////////////////////////////////////////////////////////
// test functions

//----< demo requirement #1 >------------------------------------------

bool Test::testR1()
{
	Utilities::title("Demonstrating Requirement #1");
	std::cout << "\n   *" << typeid(std::function<bool()>).name()
		<< ", declared in this function, "
		<< "\n   *is only valid for C++11 and later versions.";
	putLine();
	return true; // would not compile unless C++11
}

//----< demo requirement #2 >------------------------------------------

bool Test::testR2()
{
	Utilities::title("Demonstrating Requirement #2");
	std::cout << "\n   *A visual examination of all the submitted code "
		<< "will show only\n   *use of streams and operators new and delete.";
	putLine();
	return true;
}

//----< demo first part of requirement #3 >----------------------------

bool Test::testR3a()
{
	Utilities::title("Demonstrating Requirement #3a - creating DbElement");
	std::cout << "\n   *Creating a db element with key \"CPP Language\":";

	DbElement<std::string> demoElem;

	demoElem.name("CPP Language").descrip("Bjarne Stroustrup").dateTime(DateTime().now()).payLoad("Addison-Wesley Professional");

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

	strDb["CPPL"] = demoElem;
	putLine();
	return true;
}
//----< demo second part of requirement #3 >---------------------------

bool Test::testR3b()
{
	Utilities::title("Demonstrating Requirement #3b - creating DbCore <std::string>");
	putLine();
	std::cout << "\n   *showing all the database elements:";
	showDb(strDb);
	DbElement<std::string> demoElem;

	// add element EMCPP
	demoElem.name("Modern CPP").descrip("Scott Meyers").payLoad("O'Reilly Media");
	strDb["EMCPP"] = demoElem;
	if (!strDb.contains("EMCPP"))
		return false;

	// add element SICP
	demoElem.name("SICP").descrip("Harold Abelson").payLoad("The MIT Press");
	strDb["SICP"] = demoElem;

	// add element CPPP
	demoElem.payLoad("Addison-Wesley Professional").descrip("Stanley B. Lippman").name("CPP Primer");
	strDb["CPPP"] = demoElem;

	if (strDb.size() != 4)
		return false;

	putLine();
	std::cout << "\n   *after adding elements with keys: ";
	std::cout << "\n   *showing all the database elements:";
	showDb(strDb);
	putLine();

	// add children
	std::cout << "\n   *make two new elements children of element with key \"CPPL\"";
	strDb["CPPL"].children().push_back("CPPP");
	strDb["CPPL"].children().push_back("EMCPP");

	std::cout << "\n   *show element CPPL";
	showHeader();
	showElem(strDb["CPPL"]);

	// display the results
	putLine();
	std::cout << "\n   *showing all the database elements:";
	showDb(strDb);
	putLine();
	std::cout << "\n   *database keys are: ";
	showKeys(strDb);

	putLine();
	return true;
}

bool Test::testR4a()
{
	Utilities::title("Demonstrating Requirement #4a - Add pairs");

	// add element CA
	DbElement<std::string> demoElem;
	demoElem.name("Comp Arch").descrip("John L. Hennessy").payLoad("Morgan Kaufmann");
	strDb.addElem("CA", demoElem);
	if (!strDb.contains("CA"))
		return false;
	// add element TC
	strDb.addElem("TC", "A Tour of CPP", "Bjarne Stroustrup", "Addison-Wesley");
	if (!strDb.contains("TC"))
		return false;
	putLine();
	std::cout << "\n   *showing all the database elements:";
	std::cout << "\n   *two elems(CA & TC) added:";
	showDb(strDb);
	putLine();

	return true;

}

bool Test::testR4b()
{
	Utilities::title("Demonstrating Requirement #4b - Delete pairs");
	strDb.addChild("TC", "CA");
	std::cout << "\n   *showing all the database elements:";
	showDb(strDb);
	// delete element CA
	strDb.deleteElem("CA");

	if (strDb.contains("CA")) return false;

	std::cout << "\n   *showing all the database elements:";
	std::cout << "\n   *CA deleted:";
	std::cout << "\n   *CA child relationship is checked while showing, CA is no more child of TC:";
	showDb(strDb);
	putLine();
	return true;
}

bool Test::testR5()
{
	Utilities::title("Demonstrating Requirement #5 - Edit text matadata, Edit relationships, replacement");

	// edit element TC by name and description
	strDb.editTextMata("TC", "name", "A Tour of CPP 2");
	strDb.editTextMata("TC", "descrip", "Bjarne Stroustrup 2");
	if (strDb["TC"].name() != "A Tour of CPP 2") return false;
	if (strDb["TC"].descrip() != "Bjarne Stroustrup 2") return false;
	putLine();
	std::cout << "\n   *showing all the database elements:";
	std::cout << "\n   *Element TC's name and description have been changed:";
	showDb(strDb);
	putLine();

	// edit element CPPL by date
	DateTime date("Wed Jan 31 22:52:40 2018");
	strDb.editDatetime("CPPL", date);
	putLine();
	std::cout << "\n   *Element CPPL's dateTime edited";
	showHeader();
	showElem(strDb["CPPL"]);

	// add child TC to CPPL and detele child EMCPP from CPPL
	putLine();
	std::cout << "\n   *add child TC to CPPL";
	strDb.addChild("CPPL", "TC");
	showHeader();
	showElem(strDb["CPPL"]);
	putLine();
	std::cout << "\n   *delete child EMCPP from CPPL";
	strDb.deleteChild("CPPL", "EMCPP");
	showHeader();
	showElem(strDb["CPPL"]);

	// replace TC element with a new element
	DbElement<std::string> demoElem2;
	demoElem2.name("CPP Templates").descrip("David Vandevoorde").payLoad("Addison-Wesley Professional");
	strDb.replaceElem("TC", demoElem2);
	putLine();
	std::cout << "\n   *showing all the database elements:";
	std::cout << "\n   *TC replaced by element whose name is CPP Templates";
	showDb(strDb);
	putLine();

	return true;
}

bool Test::testR6()
{
	Utilities::title("Demonstrating Requirement #6 - Basic Query: key, children, matatext, time interval");
	std::cout << "\n   ** Query payload is in requirement 9 " << std::endl;
	std::cout << "\n   *showing all the database elements:";
	showDb(strDb);
	strDb.addElem("OCPP", "Optimized CPP", "Kurt Guntheroth", "O'Reilly Media");

	//query exact key
	Query<std::string> q1(strDb);
	std::cout << "\n   *query key: showing selected key whose key is OCPP, shall return OCPP:" << std::endl;
	q1.selectKey("OCPP").show();
	Condition c1;
	c1.name("SICP");
	q1.reset();
	//query name
	std::cout << "\n   *query name: showing selected name which is Modern CPP, shall return EMCPP:" << std::endl;
	q1.select(c1).show();
	Condition c2;
	c2.name("Modern CPP");
	Condition c3;
	c3.description(".*St.*");
	Query<std::string> q2(strDb);
	q1.reset();
	std::cout << "\n   *query description: showing selected key with \'St\' inside description, shall return CPPL CPPP:" << std::endl;
	q1.select(c3).show();

	Condition c4;
	c4.key(".*CPP.*");

	//query key using regular expression
	std::cout << "\n   *query key inside: selecte all keys with CPP inside, shall return CPPL EMCPP CPPP OCPP" << std::endl;
	q2.select(c4).show();

	// query key with some child
	Query<std::string> q4(strDb);
	std::cout << "\n   *Query child TC, shall return its parent CPPL" << std::endl;
	q4.selectKeysWithChild("TC").show();

	q4.reset();

	//query key within a time interval
	std::cout << "\n   *query time interval: select date bewteen two time: Wed Jan 31 22:52:00 2018 and Wed Jan 31 22:53:40 2018, shall return CPPL" << std::endl;
	q4.selectDate(DateTime("Wed Jan 31 22:52:00 2018"), DateTime("Wed Jan 31 22:53:40 2018")).show();
	
	putLine();
	return true;
}

bool TEST::Test::testR7()
{
	Utilities::title("Demonstrating Requirement #7 - Query AND and OR");
	std::cout << "\n   *showing all the database elements:";
	showDb(strDb);
	Condition c1;
	c1.name(".*CPP.*");
	Condition c2;
	c2.description(".*Bjarne Stroustrup.*");
	Query<std::string> q1(strDb);
	q1.select(c1);
	Query<std::string> q2(strDb);
	q2.from(q1.keys());
	//AND query
	putLine();
	std::cout << "\n   *AND query with name with CPP inside and descrip with Bjarne Stroustrup, shall return CPPL" << std::endl;
	q2.select(c2).show();

	Query<std::string> q3(strDb);
	q3.select(c1);
	Query<std::string> q4(strDb);
	q4.select(c2);
	//OR query
	std::cout << "\n   *OR query with name with CPP inside or descrip with Bjarne Stroustrup, shall return CPPL CPPP EMCPP OCPP TC" << std::endl;
	q4.unionFrom(q3.keys(), q4.keys()).show();
	putLine();
	return true;
}

bool Test::testR8()
{
	Utilities::title("Demonstrating Requirement #8 - Persistence");

	std::cout << "\n   *showing all the database elements:";
	showDb(strDb);
	putLine();
	// save db to xml
	std::cout << "\n   *Save DB to books.xml" << std::endl;
	strDb.SaveToXML("../XMLFiles/books.xml");

	// read from xml
	std::cout << "\n   *Read from withFileInfo.xml" << std::endl;
	fileDb.ReadFromXML("../XMLFiles/withFileInfo.xml");
	std::cout << "\n   *show database from xml file with payload of fileInfo" << std::endl;
	showDb(fileDb);
	putLine();
	// save db to xml
	fileDb.SaveToXML("../XMLFiles/withFileInfo2.xml");
	// augment
	fileDb.ReadFromXML("../XMLFiles/withFileInfo3.xml");
	std::cout << "\n   *augment the database from withFileInfo3.xml, one new item added" << std::endl;
	showDb(fileDb);
	putLine();
	return true;
}

bool TEST::Test::testR9()
{
	Utilities::title("Demonstrating Requirement #9 - Implement FileInfo payload and Query payload");

	PAYLOAD::FileInfo f0;
	f0.filePath() = "DEMO PATH";
	f0.addCate("DEMO CAT1");
	f0.addCate("DEMP CAT2");

	std::cout << "\n   *showing new payload: fileinfo with filepath and categories" << std::endl;
	std::cout << f0 << std::endl;

	std::cout << "\n   *query payload FileInfo: showing all the database<PAYLOAD::FileInfo> elements:";
	showDb(fileDb);
	putLine();

	Query<PAYLOAD::FileInfo> q1(fileDb);
	PAYLOAD::FileInfo f1;
	f1.addCate("cpp");
	std::cout << "\n   *query payload with category CPP, shall return CPPL EMCPP TC OCPP CPPP" << std::endl;
	q1.selectWithPayLoad(f1).show();
	q1.reset();
	PAYLOAD::FileInfo f2;
	f2.filePath() = ".*cpp.*";
	// query payload(fileInfo)
	std::cout << "\n   *query payload with path containing cpp, shall return EMCPP TC OCPP CPPP" << std::endl;
	q1.selectWithPayLoad(f2).show();

	std::cout << "\n   *query payload string: showing all the database<std::string> elements:";
	showDb(strDb);
	putLine();
	Query<std::string> q3(strDb);
	// query payload(string)
	std::cout << "\n   *query string payload, all keys with Addison inside payload, shall return CPPL CPPP TC" << std::endl;
	q3.selectWithPayLoad(".*Addison.*").show();

	putLine();
	return true;
}

bool TEST::Test::testR10()
{
	Utilities::title("Demonstrating Requirement #10 - Packages");
	putLine();
	std::cout << "   This projects includes following packages: \n"
		<< "    Executvie, DBcore, Query, Test, Payload, DateTime, XmlDocument(Persistence)" << std::endl;

	return true;
}

bool TEST::Test::testR11()
{
	Utilities::title("Demonstrating Requirement #11 - Project structure in XML");
	putLine();

	std::cout << "   The structure XML file is loacted in the  \"GradingFiles(Require11n13)/project.xml" << std::endl;

	NoSqlDb::DbCore<std::string> structDB;
	structDB.ReadFromXML("../GradingFiles(Require11n13)/project.xml");
	std::cout << "\n   *showing all the database elements:";
	showDb(structDB);
	putLine();
	return true;
}

bool TEST::Test::testR13()
{
	Utilities::title("Demonstrating Requirement #13 - Package Diagram");
	putLine();

	std::cout << "   The package Diagram is loacted in the \"GradingFiles(Require11n13)/PackageDiagram.pdf\" " << std::endl;

	return true;
}

#ifdef TEST_TEST
int main() {
	TEST::Test t;
	t.testR1();
	t.testR2();
	t.testR3a();
	t.testR3b();
	t.testR4a();
	t.testR4b();
	t.testR5();
	t.testR6();
	t.testR7();
	t.testR8();
	t.testR9();
	t.testR10();
	t.testR11();
	t.testR13();
	std::cout << "test sucseed" << std::endl;
	getchar();
	getchar();
	return 0;
}
#endif // TEST_TEST
