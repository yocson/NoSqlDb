#pragma once
/////////////////////////////////////////////////////////////////////
// Test.h - NoSql Test Functions                                   //
// ver 2.2                                                         //
// Cheng Wang, CSE687 - Object Oriented Design, Spring 2018        //
// cwang76@syr.edu												   //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides test functions:
* - to demostrate all requirements
* There are two static member data:
* - db with string as payload
* - testDb with fileInfo as payload
*
* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* DateTime.h, DateTime.cpp
* Utilities.h, Utilities.cpp
* PayLoad.h, PayLoad.cpp
*
* Maintenance History:
* --------------------
* ver 2.2 : Feb 4, 2018
* - add a bunch of test functions
* ver 2.1 : Feb 3, 2018
* - add static database 
* ver 2.0 : Feb 2, 2018
* - classize Test
* ver 1.0 : Jan 30, 2018
* - first release
*/

#include "../PayLoad/PayLoad.h"

namespace TEST 
{
	class Test 
	{
	public:
		static bool testR1();
		static bool testR2();
		static bool testR3a();
		static bool testR3b();
		static bool testR4a();
		static bool testR4b();
		static bool testR5();
		static bool testR6();
		static bool testR7();
		static bool testR8();
		static bool testR9();
		static bool testR10();
		static bool testR11();
		static bool testR13();

	private:
		static NoSqlDb::DbCore<PAYLOAD::FileInfo> fileDb;
		static NoSqlDb::DbCore<std::string> strDb;
	};

}

