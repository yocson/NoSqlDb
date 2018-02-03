#pragma once

namespace TEST {
	class Test {
	public:
		bool testR1();
		bool testR2();
		bool testR3a();
		bool testR3b();
		bool testR4();
		bool testR5();
		bool testR6();
		bool testR8();

	private:
		static NoSqlDb::DbCore<FileInfo> testDb;
		static NoSqlDb::DbCore<std::string> db;
	};

}

