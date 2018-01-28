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
Condition& Condition::datetime(std::string re)
{
	datetime_re = re;
	return *this;
}
Condition& Condition::name(std::string re)
{
	name_re = re;
	return *this;
}