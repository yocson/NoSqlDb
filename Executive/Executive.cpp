#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include "../Query/Query.h"
#include "../Test/Test.h"
#include <iostream>
#include <iomanip>
#include <functional>


#ifdef EXECUTIVE

using namespace TEST;

int main()
{
	Utilities::Title("Testing DbCore - Computer Books database");
	std::cout << "\n  Cheng Wang, cwang76@syr.edu";
	Utilities::putline();
	TestExecutive ex;

	// define test structures with test function and message
	TestExecutive::TestStr ts1{ Test::testR1, "Use C++11" };
	TestExecutive::TestStr ts2{ Test::testR2, "Use streams and new and delete" };
	TestExecutive::TestStr ts3a{ Test::testR3a, "Create DbElement<std::string>" };
	TestExecutive::TestStr ts3b{ Test::testR3b, "Create DbCore<std::string>" };
	TestExecutive::TestStr ts4a{ Test::testR4a, "add pair" };
	TestExecutive::TestStr ts4b{ Test::testR4b, "delete pair" };
	TestExecutive::TestStr ts5{ Test::testR5, "edit element" };
	TestExecutive::TestStr ts6{ Test::testR6, "basic query" };
	TestExecutive::TestStr ts7{ Test::testR7, "AND and OR query" };
	TestExecutive::TestStr ts8{ Test::testR8, "Persistence" };
	TestExecutive::TestStr ts9{ Test::testR9, "fileInfo and query payload" };
	TestExecutive::TestStr ts10{ Test::testR10, "packages" };
	TestExecutive::TestStr ts11{ Test::testR11, "structure XML" };
	TestExecutive::TestStr ts13{ Test::testR13, "package diagram" };
	// register test structures with TestExecutive instance, ex
	ex.registerTest(ts1);
	ex.registerTest(ts2);
	ex.registerTest(ts3a);
	ex.registerTest(ts3b);
	ex.registerTest(ts4a);
	ex.registerTest(ts4b);
	ex.registerTest(ts5);
	ex.registerTest(ts6);
	ex.registerTest(ts7);
	ex.registerTest(ts8);
	ex.registerTest(ts9);
	ex.registerTest(ts10);
	ex.registerTest(ts11);
	ex.registerTest(ts13);
	// run tests
	bool result = ex.doTests();
	if (result == true)
		std::cout << "\n  all tests passed";
	else
		std::cout << "\n  at least one test failed";
	Utilities::putline(2);
	getchar();
	getchar();
	return 0;
}
#endif