#pragma once

namespace TEST {
	class Test {
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

