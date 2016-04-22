#include <string>
#include <iomanip>
#include <iostream>
#include "lex_compare.hpp"

class Person;
bool test_predicate(const Person& lhs, const Person& rhs);

class Person
{
public:
	Person(std::string surname, int age, char initial, int income)
		:surname(std::move(surname))
		,age(age)
		,initial(initial)
		,income(income)
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

	int getIncome() const
	{
		return income;
	}

	bool operator<(const Person& rhs) const
	{
		return lco::less_than(*this, rhs,
			&Person::surname,                                     // member
			&Person::getAge,                                      // method
			[](const Person& p) -> char {return p.getInitial();}, // functor
			LCOPRED(Person, test_predicate)                       // predicate functor
		);
	}

private:
	std::string surname;
	int age;
	char initial;
	int income;

	friend std::ostream& operator<<(std::ostream& os, const Person& p);
};

bool test_predicate(const Person& lhs, const Person& rhs)
{
	return lhs.getIncome() < rhs.getIncome();
}

std::ostream& operator<<(std::ostream& os, const Person& p)
{
	os << "{" << p.surname << ", " << p.age << ", " << p.initial << ", " << p.income << "}";
	return os;
}

int main()
{
	const Person people[5] = {
		Person("Smith", 43, 'B', 45000),
		Person("Smith", 43, 'J', 52000),
		Person("Smith", 9, 'B', 0),
		Person("Doe", 19, 'J', 20000),
		Person("Doe", 43, 'J', 60000)
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
