#ifndef LCO_LEX_COMPARE_HPP
#define LCO_LEX_COMPARE_HPP

namespace lco
{

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

template <typename Object, typename Member>
bool LessThan(const Object& lhs, const Object& rhs, const Member& field)
{
	return impl::LessThan(lhs, rhs, field);
}

namespace impl
{

template <typename Object, typename MethodReturn>
bool LessThan(const Object& lhs, const Object& rhs, MethodReturn(Object::*method)() const)
{
	return (lhs.*method)() < (rhs.*method)();
}

template <typename Object, typename MemberType>
bool LessThan(const Object& lhs, const Object& rhs, MemberType Object::*member)
{
	return (lhs.*member) < (rhs.*member);
}


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

template <typename Object, typename Functor>
bool LessThan(const Object& lhs, const Object& rhs, const Pred<Object, Functor>& f)
{
	return f(lhs, rhs);
}

template <typename Object, typename Functor>
bool LessThan(const Object& lhs, const Object& rhs, const Functor& f)
{
	return f(lhs) < f(rhs);
}

} // impl
} // lco

#define LCOPRED(Object, functor) lco::impl::Pred<Object, decltype(&functor)>(&functor)

#endif // LCO_LEX_COMPARE_HPP