#pragma once
/////////////////////////////////////////////////////////////////////
// Test.h - NoSql Test Functions                                   //
// ver 1.0                                                         //
// Cheng Wang, CSE687 - Object Oriented Design, Spring 2018        //
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
		bool testR1();
		bool testR2();
		bool testR3a();
		bool testR3b();
		bool testR4a();
		bool testR4b();
		bool testR5();
		bool testR6();
		bool testR7();
		bool testR8();
		bool testR9();

	private:
		static NoSqlDb::DbCore<PAYLOAD::FileInfo> fileDb;
		static NoSqlDb::DbCore<std::string> strDb;
	};

}

