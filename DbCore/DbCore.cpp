/////////////////////////////////////////////////////////////////////
// DbCore.cpp - Implements NoSql database prototype                //
// ver 1.0                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018       //
/////////////////////////////////////////////////////////////////////

#include "DbCore.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include "../Query/Query.h"
#include <iostream>
#include <iomanip>
#include <functional>

using namespace NoSqlDb;

//----< reduce the number of characters to type >----------------------


//----< test stub >----------------------------------------------------

#ifdef TEST_DBCORE

using namespace Utilities;

int main()
{
  Utilities::Title("Testing DbCore - He said, she said database");
  putLine();

  TestExecutive ex;

  // define test structures with test function and message

  TestExecutive::TestStr ts1{ testR1, "Use C++11" };
  TestExecutive::TestStr ts2{ testR2, "Use streams and new and delete" };
  TestExecutive::TestStr ts3a{ testR3a, "Create DbElement<std::string>" };
  TestExecutive::TestStr ts3b{ testR3b, "Create DbCore<std::string>" };
  TestExecutive::TestStr ts4{ testR4, "add element" };
  TestExecutive::TestStr ts5{ testR5, "add element" };
  TestExecutive::TestStr ts6{ testR6, "query" };

  // register test structures with TestExecutive instance, ex

  ex.registerTest(ts1);
  ex.registerTest(ts2);
  ex.registerTest(ts3a);
  ex.registerTest(ts3b);
  ex.registerTest(ts4);
  ex.registerTest(ts5);
  ex.registerTest(ts6);

  // run tests

  bool result = ex.doTests();
  if (result == true)
    std::cout << "\n  all tests passed";
  else
    std::cout << "\n  at least one test failed";

  putLine(2);
  return 0;
}
#endif
