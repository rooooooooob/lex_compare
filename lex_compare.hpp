#ifndef LCO_LEX_COMPARE_HPP
#define LCO_LEX_COMPARE_HPP

namespace lco
{

/**
 * Lexicographically compares two objects
 * @param lhs Left hand side object to compare
 * @param rhs Right hand side object to compare
 * @param field First field of the objects to compare
 * @param fields The rest of the fields to compare
 * @return true if lhs is lexicographically before rhs
 */
template <typename Object, typename Field, typename... Fields>
bool LessThan(const Object& lhs, const Object& rhs, const Field& field, Fields... fields)
{
	if (LessThan(lhs, rhs, field))
	{
		return true;
	}
	else if (LessThan(rhs, lhs, field))
	{
		return false;
	}
	return LessThan(lhs, rhs, fields...);
}

/**
 * Lexicographically compares two objects
 * @param lhs Left hand side object to compare
 * @param rhs Right hand side object to compare
 * @param field The field of the objects to compare
 * @return true if lhs is lexicographically before rhs
 */
template <typename Object, typename Field>
bool LessThan(const Object& lhs, const Object& rhs, const Field& field)
{
	return impl::LessThan(lhs, rhs, field);
}

/**
 * A predicate comparator that invokes the wrapped functor
 * on the objects to compare and expects the functor to return
 * true if lhs is lexicographically smaller than rhs along whatever
 * custom comparisons the user wants. Use this only if your compairons
 * cannot be directly expressed as comparing two fields of an object
 */
template <typename Object, typename Functor>
class Pred
{
public:
	Pred(Functor f)
		:f(std::move(f))
	{
	}

	bool operator()(const Object& lhs, const Object& rhs) const
	{
		return f(lhs, rhs);
	}

private:
	Functor f;
};

// Helper macro for defining predicates
#define LCOPRED(Object, functor) lco::Pred<Object, decltype(&functor)>(&functor)

// Actual implementations of comparisons on a type-by-type basis to allow heterogenous support of
// fields, methods, and custom functions
namespace impl
{

/**
 * Compares the result of a getter function of lhs to that of rhs. ie lhs.method() < rhs.method()
 */
template <typename Object, typename MethodReturn>
bool LessThan(const Object& lhs, const Object& rhs, MethodReturn(Object::*method)() const)
{
	return (lhs.*method)() < (rhs.*method)();
}

/**
 * Compare a field of lhs to a field of rhs using member variables ie lhs.field < rhs.field
 */
template <typename Object, typename MemberType>
bool LessThan(const Object& lhs, const Object& rhs, MemberType Object::*member)
{
	return (lhs.*member) < (rhs.*member);
}

/**
 * Compares lhs to rhs using a special predicate which will assume that lhs < rhs if f(lhs, rhs) = true
 */
template <typename Object, typename Functor>
bool LessThan(const Object& lhs, const Object& rhs, const Pred<Object, Functor>& f)
{
	return f(lhs, rhs);
}

/**
 * Lexicographically compares lhs to rhs by the result of the passed in functor ie f(lhs) < f(rhs)
 */
template <typename Object, typename Functor>
bool LessThan(const Object& lhs, const Object& rhs, const Functor& f)
{
	return f(lhs) < f(rhs);
}

} // impl
} // lco

#endif // LCO_LEX_COMPARE_HPP