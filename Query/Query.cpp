#include "Query.h"

//----< set description >----------------------

Condition& Condition::description(std::string re)
{
	description_re = re;
	return *this;
}

//----< set dateTime >----------------------
Condition& Condition::datetime(DateTime date)
{
	date_ = date;
	dateSelected = true;
	return *this;
}

//----< set name >----------------------
Condition& Condition::name(const std::string re)
{
	name_re = re;
	return *this;
}

//----< set key >----------------------
Condition& Condition::key(std::string re)
{
	key_re = re;
	return *this;
}


//----< test stub >----------------------

#ifdef TEST_QUERY
int main()
{
	NoSqlDb::DbCore<std::string> strDb;
	strDb.addElem("TC", "A Tour of CPP", "Bjarne Stroustrup", "Addison-Wesley");
	strDb.addElem("MCPP", "Modern CPP", "Scott Meyers", "O'Reilly Media");
	strDb.addElem("CPPP", "CPP Primer", "Stanley B. Lippman", "Addison-Wesley Professional");
	strDb.addElem("CPPL", "CPP Language", "Bjarne Stroustrup", "Addison-Wesley Professional");
	strDb.addChild("CPPL", "TC");
	std::cout << "\n  Showing whole Database:" << std::endl;
	NoSqlDb::showDb(strDb);
	Query<std::string> q1(strDb);
	std::cout << "\n  Showing selected key whose key is TC:" << std::endl;
	q1.selectKey("TC").show();
	q1.reset();
	std::cout << "\n  Showing selected key whose key has CPP:" << std::endl;
	q1.selectKey(".*CPP.*").show();
	Condition c1;
	c1.name("CPP Language");
	q1.reset();
	std::cout << "\n  Showing selected key whose name is CPP Language:" << std::endl;
	q1.select(c1).show();
	q1.reset();
	std::cout << "\n  Showing selected description with Bjarne inside:" << std::endl;
	q1.selectDescription(".*Bjarne.*").show();
	q1.reset();
	std::cout << "\n  Showing keys between Wed Jan 31 22:52:40 2018 and now:" << std::endl;
	q1.selectDate(DateTime("Wed Jan 31 22:52:40 2018")).show();
	Condition c2;
	c2.description(".*Bjarne.*");
	q1.reset();
	std::cout << "\n  AND query name with CPP Language and Bjarne inside description:" << std::endl;
	q1.select(c1).select(c2).show();
	Query<std::string> q2(strDb);
	Query<std::string> q3(strDb);
	q2.selectChildren("TC");
	q3.selectWithPayLoad(".*Addison.*");
	std::cout << "\n  OR query key with TC as a child and Addison inside payload(string):" << std::endl;
	q1.unionFrom(q2.keys(), q3.keys()).show();
	getchar();
	getchar();
	return 0;
}
#endif