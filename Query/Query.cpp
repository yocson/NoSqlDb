#include "Query.h"

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

Condition& Condition::name(const std::string re)
{
	name_re = re;
	return *this;
}

Condition& Condition::key(std::string re)
{
	key_re = re;
	return *this;
}

//union condtions: Wrong
Condition Condition::operator+(const Condition& conds) const
{
	Condition temp;
	std::vector<std::string> res(3);
	std::vector<std::string> para1 = { name_re, description_re, key_re };
	std::vector<std::string> para2 = { conds.name_re, conds.description_re, conds.key_re };
	for (size_t i = 0; i < para1.size(); i++) {
		if (para1[i].length()!=0 && para2[i].length()!=0) {
			res[i] = "(" + para1[i] + ")" + "|" + "(" + para2[i] + ")";
		}
		else {
			res[i] = (para1[i].length() == 0) ? para2[i] : para1[i];
		}
	}
	temp.name_re = res[0];
	temp.description_re = res[1];
	temp.key_re = res[2];
	return temp;
}

#ifdef TEST_QUERY
int main()
{
	return 0;
}
#endif