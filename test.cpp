#include <string>
#include <iomanip>
#include <iostream>

struct Person
{
	std::string surname;
	int age;
	char initial;
};

template <typename Object, typename Member, typename... Members>
bool less_than(const Object& lhs, const Object& rhs, Member member, Members... members)
{
	if (lhs.*member < rhs.*member)
	{
		return true;
	}
	else if (lhs.*member > rhs.*member)
	{
		return false;
	}
	return less_than<Object, Members...>(lhs, rhs, members...);
}

template <typename Object, typename Member>
bool less_than(const Object& lhs, const Object& rhs, Member member)
{
	return (lhs.*member < rhs.*member);
}

bool operator<(const Person& lhs, const Person& rhs)
{
	return less_than(lhs, rhs, &Person::surname, &Person::age, &Person::initial);
}

std::ostream& operator<<(std::ostream& os, const Person& p)
{
	os << "{" << p.surname << ", " << p.age << ", " << p.initial << "}";
	return os;
}

int main()
{
	const Person people[5] = {
		{ "Smith", 43, 'B' },
		{ "Smith", 43, 'J' },
		{ "Smith", 9, 'B' },
		{ "Doe", 19, 'J' },
		{ "Doe", 43, 'J' }
	};
	for (const Person& lhs : people)
	{
		for (const Person& rhs : people)
		{
			std::cout << lhs << " < " << rhs << " ?   " << std::boolalpha << (lhs < rhs) << std::endl;
		}
	}
	return 0;
}