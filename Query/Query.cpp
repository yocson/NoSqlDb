#include "Query.h"

Condition& Condition::author(std::string re)
{
	author_re = re;
	return *this;
}

Condition& Condition::description(std::string re)
{
	description_re = re;
	return *this;
}
Condition& Condition::datetime(DateTime date)
{
	date_ = date;
	dateSelected = true;
	return *this;
}
Condition& Condition::name(std::string re)
{
	name_re = re;
	return *this;
}

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