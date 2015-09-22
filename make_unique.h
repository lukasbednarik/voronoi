#ifndef MAKEUNIQUE_H
#define MAKEUNIQUE_H

#include <memory>


/// Convenience function. Replace with std::make_unique in C++14.
template <class T>
std::unique_ptr<T> make_unique() 
{
	return std::unique_ptr<T>(new T());
}

/// Convenience function. Replace with std::make_unique in C++14.
template <class T, class Arg1>
std::unique_ptr<T> make_unique(Arg1 && arg1) 
{
	return std::unique_ptr<T>(new T(std::forward<Arg1>(arg1)));
}

/// Convenience function. Replace with std::make_unique in C++14.
template <class T, class Arg1, class Arg2>
std::unique_ptr<T> make_unique(Arg1 && arg1, Arg2 && arg2) 
{
	return std::unique_ptr<T>(new T(std::forward<Arg1>(arg1), std::forward<Arg2>(arg2)));		
}


#endif  // MAKEUNIQUE_H
