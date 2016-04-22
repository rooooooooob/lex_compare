#ifndef LCO_LEX_COMPARE_HPP
#define LCO_LEX_COMPARE_HPP

namespace lco
{

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

template <typename Object, typename Member, typename... Members>
bool LessThan(const Object& lhs, const Object& rhs, const Member& member, Members... members)
{
	if (LessThan(lhs, rhs, member))
	{
		return true;
	}
	else if (LessThan(rhs, lhs, member))
	{
		return false;
	}
	return LessThan<Object, Members...>(lhs, rhs, members...);
}

template <typename Object, typename Member>
bool LessThan(const Object& lhs, const Object& rhs, const Member& member)
{
	return impl::LessThan(lhs, rhs, member);
}

namespace impl
{

template <typename Object, typename MethodReturn>
bool LessThan(const Object& lhs, const Object& rhs, MethodReturn(Object::*method)() const)
{
	return (lhs.*method)() < (rhs.*method)();
}

template <typename Object, typename Functor>
bool LessThan(const Object& lhs, const Object& rhs, const Functor& f)
{
	return f(lhs) < f(rhs);
}

template <typename Object, typename MemberType>
bool LessThan(const Object& lhs, const Object& rhs, MemberType Object::*member)
{
	return (lhs.*member) < (rhs.*member);
}

template <typename Object, typename Functor>
bool LessThan(const Object& lhs, const Object& rhs, const Pred<Object, Functor>& f)
{
	return f(lhs, rhs);
}

} // impl

} // lco

#ifndef LCOPRED
	#define LCOPRED(Object, functor) lco::Pred<Object, decltype(&functor)>(&functor)
#endif // LCOPRED

#endif // LCO_LEX_COMPARE_HPP