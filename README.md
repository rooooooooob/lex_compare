# Lex Compare

Templated header-only library for lexicographically comparing objects.

Tired of writing your own less-than operators for classes? Wish you could use
something like std::lexicographical_compare to automatically generate it?

Now that C++ has variadic templates you can! Just include this header-only
library and specify which fields, methods or custom functions you want your
classes to be compared with!

### Supported compare types:

* Member variables.  usage: &Class::var.       result: lhs.var < rhs.var
* Member methods.    usage: &Class::method. result: lhs.method() < rhs.method()
* Custom functions.  usage: f.                 result: f(lhs) < f(rhs)
* Custom predicates. usage: LCOPRED(Class, f). result: f(lhs, rhs)

Example:
```c++
struct Dog
{
    int getBones() const;
    int toyCount() const;
    int age;
};

bool customPredicate(const Dog& lhs, const Dog& rhs);

bool operator<(const Dog& lhs, const Dog& rhs)
{
    return lco::LessThan(
        lhs,
        rhs,
        &Dog::age,                    // member variable
        &Dog::getBones,               // member method
        [](const Dog& d) -> int {
            return d.toyCount();      // custom function
        },
        LCOPRED(Dog, customPredicate) // custom predicate
    );
}
```

or to create a functor:
```c++
auto comp = lco::Functor<Dog>::make(&Dog::age, &Dog::getBones, &someFunction);
// call it
comp(dog1, dog2);
// or pass it to STL containers/etc
std::map<std::string, Dog, decltype(comp)> dogs(comp);
```

Working examples are shown in test.cpp

Include just lex_compare.hpp for calling less-than or lex_compare_functor.hpp
(which needs lex_compare.hpp) if you need the comparison in functor form,
or you could just use std::bind on it I guess.
