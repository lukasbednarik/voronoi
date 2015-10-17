// The MIT License (MIT)
//
// Copyright (c) 2015 Lukáš Bednařík
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


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
