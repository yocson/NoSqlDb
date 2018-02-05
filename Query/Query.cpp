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


#ifdef TEST_QUERY
int main()
{
	return 0;
}
#endif