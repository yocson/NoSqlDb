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
	Test tt;

	// define test structures with test function and message

	TestExecutive::TestStr ts1{ std::bind(&Test::testR1, tt), "Use C++11" };
	TestExecutive::TestStr ts2{ std::bind(&Test::testR2, tt), "Use streams and new and delete" };
	TestExecutive::TestStr ts3a{ std::bind(&Test::testR3a, tt), "Create DbElement<std::string>" };
	TestExecutive::TestStr ts3b{ std::bind(&Test::testR3b, tt), "Create DbCore<std::string>" };
	TestExecutive::TestStr ts4{ std::bind(&Test::testR4, tt), "add element" };
	TestExecutive::TestStr ts5{ std::bind(&Test::testR5, tt), "add element" };
	TestExecutive::TestStr ts6{ std::bind(&Test::testR6, tt), "query" };
	TestExecutive::TestStr ts8{ std::bind(&Test::testR8, tt), "xml" };

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