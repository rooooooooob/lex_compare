# Lex Compare

Templated header-only library for lexicographically comparing objects.

Tired of writing your own less-than operators for classes? Wish you could use
something like std::lexicographical_compare to automatically generate it?

Now that C++ has variadic templates you can! Just include this header-only
library and specify which fields, methods or custom functions you want your
functions to be compared with!

### Supported compare types:

* Member variables.  usage: &Class::var.    result: lhs.var < rhs.var
* Member methods.    usage: &Class::method. result: lhs.method() < rhs.method()
* Custom functions.  usage: f.              result: f(lhs) < f(rhs)
* Custom predicates. usage: LCOPRED(f).     result: f(lhs, rhs)

Example:
```c++
lco::LessThan(
    &Dog::age,                                      // member variable
    &Dog::getBones,                                 // member method
    [](const Dog& d) -> int {
        return d.toyCount();                        // custom function
    },
    [](const Dog& lhs, const Dog& rhs) -> bool
        return lhs.catsChased < rhs.catsChased;     // custom predicate
    }
);
```

Working examples are shown in test.cpp

Include just lex_compare.hpp for calling less-than or lex_compare_functor.hpp
(which needs lex.compare.hpp) if you need the comparison in functor form,
or you could just use std::bind on it I guess.
