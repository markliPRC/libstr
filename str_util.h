#ifndef _STR_UTIL_
#define _STR_UTIL_

#include <cstdarg>
#include <cstdio>
#include <cwchar>

#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

namespace libstr
{
template <typename T>
class bstr_util
{
public:
	template <typename U>
	static U cast(const std::basic_string<T>& src)
	{
		std::basic_istringstream<T> iss(src);
		U u{};
		iss >> u;
		return u;
	}

	static std::basic_string<T> format(const T* fmt, ...);

	template <typename U>
	static std::basic_string<T> join(const std::vector<U>& src, const std::basic_string<T>& delimiter)
	{
		std::basic_ostringstream<T> oss;

		std::copy(src.begin(), src.end(), std::ostream_iterator<U, T>(oss, delimiter.c_str()));

		std::basic_string<T> str = oss.str();
		if(!str.empty())
		{
			str.resize(str.length() - delimiter.length());
		}

		return str;
	}

	template <typename U>
	static std::basic_string<T> join(const std::vector<U>& src, T delimiter)
	{
		return join(src, std::basic_string<T>(1, delimiter));
	}

	static std::basic_string<T>& replace(std::basic_string<T>& src, const std::basic_string<T>& before, const std::basic_string<T>& after)
	{
		for(typename std::basic_string<T>::size_type i = src.find(before); i != std::basic_string<T>::npos; i = src.find(before, i + after.length()))
		{
			src.replace(i, before.length(), after);
		}

		return src;
	}

	template <typename U>
	static std::vector<U> split(const std::basic_string<T>& src, const std::basic_string<T>& delimiter)
	{
		std::vector<U> dest;

		typename std::basic_string<T>::size_type start = 0;
		for(typename std::basic_string<T>::size_type i = src.find(delimiter); i != std::basic_string<T>::npos; i = src.find(delimiter, start))
		{
			dest.push_back(cast<U>(src.substr(start, i - start)));
			start = i + delimiter.length();
		}

		if(start < src.length())
		{
			dest.push_back(cast<U>(src.substr(start)));
		}

		return dest;
	}

	template <typename U>
	static std::vector<U> split(const std::basic_string<T>& src, T delimiter)
	{
		return split<U>(src, std::basic_string<T>(1, delimiter));
	}

	static std::basic_string<T>& trim_left(std::basic_string<T>& src, const std::basic_string<T>& chars)
	{
		for(typename std::basic_string<T>::iterator it = src.begin(); it != src.end(); ++it)
		{
			if(chars.find(*it) == std::basic_string<T>::npos)
			{
				src.erase(src.begin(), it);
				break;
			}
		}

		return src;
	}

	static std::basic_string<T>& trim_right(std::basic_string<T>& src, const std::basic_string<T>& chars)
	{
		for(typename std::basic_string<T>::reverse_iterator it = src.rbegin(); it != src.rend(); ++it)
		{
			if(chars.find(*it) == std::basic_string<T>::npos)
			{
				src.erase(it.base(), src.end());
				break;
			}
		}

		return src;
	}

	static std::basic_string<T>& trim(std::basic_string<T>& src, const std::basic_string<T>& chars)
	{
		return trim_right(trim_left(src, chars), chars);
	}
};

using str_util = bstr_util<char>;
using wstr_util = bstr_util<wchar_t>;

template <>
inline std::string str_util::format(const char* fmt, ...)
{
	va_list ap, ap2;

	va_start(ap, fmt);
	va_copy(ap2, ap);

	std::string str(vsnprintf(nullptr, 0, fmt, ap), '\0');

	vsprintf(&str[0], fmt, ap2);

	va_end(ap);
	va_end(ap2);

	return str;
}

template <>
inline std::wstring wstr_util::format(const wchar_t* fmt, ...)
{
	va_list ap, ap2;

	va_start(ap, fmt);
	va_copy(ap2, ap);

	std::wstring str(vswprintf(nullptr, 0, fmt, ap), L'\0');

	vswprintf(&str[0], str.length() + 1, fmt, ap2);

	va_end(ap);
	va_end(ap2);

	return str;
}
}	//libstr

#endif	//_STR_UTIL_
