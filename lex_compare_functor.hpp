#ifndef LCO_LEX_COMPARE_FUNCTOR_HPP
#define LCO_LEX_COMPARE_FUNCTOR_HPP

#include "lex_compare.hpp"
#include <tuple>

namespace lco
{

/**
 * A functor version of lco::LessThan
 * Note: Constructor can be unwieldy to use since template types cannot be
 * inferred so it is advised to use Functor<Object>::make(f1, f2, ...) instead
 * of LessThanFunctor<Object, decltype(f1), decltype(f2), ...>(f1, f2, ...)
 */
template <typename Object, typename... Fields>
class LessThanFunctor
{
public:
	/**
	 * @param fields The fields to use in Object comparison
	 */
	LessThanFunctor(Fields... fields)
		:fields(fields...)
	{
	}

	/**
	 * Compares two input objects lexicographically by the stored fields
	 * @param lhs left Object to compare
	 * @param
	 * @return true if lhs < rhs lexicographically
	 */
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

/**
 * A utility function to make construction of LessThanFunctor less verbose
 */
template <typename Object>
class Functor
{
public:
	/**
	 * Constructs a LessThanFunctor that compares by fields
	 * @param fields The fields to lexicographically comapre by
	 * @return the functor
	 */
	template <typename... Fields>
	static auto make(Fields... fields) -> LessThanFunctor<Object, Fields...>
	{
		return LessThanFunctor<Object, Fields...>(fields...);
	}
};

} // lco

#endif // LCO_LEX_COMPARE_FUNCTOR_HPP
