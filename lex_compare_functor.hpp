#ifndef LCO_LEX_COMPARE_FUNCTOR_HPP
#define LCO_LEX_COMPARE_FUNCTOR_HPP

#include "lex_compare.hpp"
#include <tuple>

namespace lco
{

template <typename Object, typename... Fields>
class LessThanFunctor
{
public:
	LessThanFunctor(Fields... fields)
		:fields(fields...)
	{
	}

	bool operator()(const Object& lhs, const Object& rhs) const
	{
		return compare<0>(lhs, rhs);
	}

private:
	
	template <std::size_t FieldIndex>
	bool compare(const Object& lhs, const Object& rhs) const
	{
		if (LessThan(lhs, rhs, std::get<FieldIndex>(fields)))
		{
			return true;
		}
		else if(LessThan(rhs, lhs, std::get<FieldIndex>(fields)))
		{
			return false;
		}
		return compare<FieldIndex>(lhs, rhs);
	}
	template <>
	bool compare<sizeof...(Fields) - 1>(const Object& lhs, const Object& rhs) const
	{
		return LessThan(lhs, rhs, std::get<sizeof...(Fields)-  1>(fields));
	}

	std::tuple<Fields...> fields;
};

template <typename Object>
class Functor
{
public:
	template <typename... Fields>
	static auto make(Fields... fields) -> LessThanFunctor<Object, Fields...>
	{
		return LessThanFunctor<Object, Fields...>(fields...);
	}
};

} // lco

#endif // LCO_LEX_COMPARE_FUNCTOR_HPP
