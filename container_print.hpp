#pragma once

#include <iostream>
#include <iterator>
#include <vector>
#include <utility>

template<template <typename...> class ContainerT, typename... T>
typename std::enable_if <!std::is_same <ContainerT<T...>, std::basic_string <T...>>::value, std::ostream>::type& 
operator<<(std::ostream& s, ContainerT<T...> t) 
{ 
    if (t.empty())
    {
        s << "[]";
        return s;
    }
    s << '[';
    auto i = std::begin(t);
    for (; i != std::end(t); ++i)
    {
        auto j = i;
        ++j;
        if (j == std::end(t))
            break;
        s << *i << ',';
    }
    //++i;
    s << *i;
    return s << ']';
}
