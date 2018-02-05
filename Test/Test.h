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
*
* Maintenance History:
* --------------------
* ver 1.1 : 19 Jan 2018
* - added code to throw exception in index operators if input key is not in database
* ver 1.0 : 10 Jan 2018
* - first release
*/

#pragma once

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
		static NoSqlDb::DbCore<FileInfo> testDb;
		static NoSqlDb::DbCore<std::string> db;
	};

}

