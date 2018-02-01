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
	Utilities::Title("Testing DbCore - He said, she said database");
	Utilities::putline();

	TestExecutive ex;

	// define test structures with test function and message

	TestExecutive::TestStr ts1{ TEST::testR1, "Use C++11" };
	TestExecutive::TestStr ts2{ TEST::testR2, "Use streams and new and delete" };
	TestExecutive::TestStr ts3a{ TEST::testR3a, "Create DbElement<std::string>" };
	TestExecutive::TestStr ts3b{ TEST::testR3b, "Create DbCore<std::string>" };
	TestExecutive::TestStr ts4{ TEST::testR4, "add element" };
	TestExecutive::TestStr ts5{ TEST::testR5, "add element" };
	TestExecutive::TestStr ts6{ TEST::testR6, "query" };
	TestExecutive::TestStr ts8{ TEST::testR8, "xml" };

	// register test structures with TestExecutive instance, ex

	ex.registerTest(ts1);
	ex.registerTest(ts2);
	ex.registerTest(ts3a);
	ex.registerTest(ts3b);
	ex.registerTest(ts4);
	ex.registerTest(ts5);
	ex.registerTest(ts6);
	ex.registerTest(ts8);

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