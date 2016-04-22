#include <string>
#include <iomanip>
#include <iostream>
#include <functional>

//#define LCO_FRIEND_FUNC friend bool lco::less_than

namespace lco
{

template <typename Object, typename Member, typename... Members>
bool less_than(const Object& lhs, const Object& rhs, const Member& member, Members... members)
{
	if (less_than(lhs, rhs, member))
	{
		return true;
	}
	else if (less_than(rhs, lhs, member))
	{
		return false;
	}
	return less_than<Object, Members...>(lhs, rhs, members...);
}

template <typename Object, typename Member>
bool less_than(const Object& lhs, const Object& rhs, const Member& member)
{
	return less_than_impl(lhs, rhs, member);
}

template <typename Object, typename MethodReturn>
bool less_than_impl(const Object& lhs, const Object& rhs, MethodReturn(Object::*method)() const)
{
	return (lhs.*method)() < (rhs.*method)();
}

template <typename Object, typename MethodReturn>
bool less_than_impl(const Object& lhs, const Object& rhs, const std::function<MethodReturn(const Object&)>& f)
{
	return f(lhs) < f(rhs);
}

template <typename Object, typename MemberType>
bool less_than_impl(const Object& lhs, const Object& rhs, MemberType Object::*member)
{
	return (lhs.*member) < (rhs.*member);
}

} // lco


class Person
{
public:
	Person(std::string surname, int age, char initial)
		:surname(std::move(surname))
		, age(age)
		, initial(initial)
	{
	}

	int getAge() const
	{
		return age;
	}

	char getInitial() const
	{
		return initial;
	}

//private:
	std::string surname;
	int age;
	char initial;
};

bool operator<(const Person& lhs, const Person& rhs)
{
	std::function<char(const Person& p)> f([](const Person& p) -> char {return p.getInitial();});
	return lco::less_than(lhs, rhs, &Person::surname, &Person::getAge, f);
}

std::ostream& operator<<(std::ostream& os, const Person& p)
{
	os << "{" << p.surname << ", " << p.age << ", " << p.initial << "}";
	return os;
}

int main()
{
	const Person people[5] = {
		Person("Smith", 43, 'B'),
		Person("Smith", 43, 'J'),
		Person("Smith", 9, 'B'),
		Person("Doe", 19, 'J'),
		Person("Doe", 43, 'J')
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