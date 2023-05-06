#ifndef _STR_CVT_
#define _STR_CVT_

#include <codecvt>
#include <locale>
#include <string>

namespace libstr
{
namespace str_cvt
{
class codecvt_ansi : public std::codecvt_byname<wchar_t, char, std::mbstate_t>
{
public:
	using std::codecvt_byname<wchar_t, char, std::mbstate_t>::codecvt_byname;

	~codecvt_ansi() = default;
};

inline std::wstring string2wstring(const std::string& src)
{
	std::wstring_convert<codecvt_ansi> cvt(new codecvt_ansi("zh_CN.gb18030"));
	return cvt.from_bytes(src);
}

inline std::string wstring2string(const std::wstring& src)
{
	std::wstring_convert<codecvt_ansi> cvt(new codecvt_ansi("zh_CN.gb18030"));
	return cvt.to_bytes(src);
}

inline std::wstring u8string2wstring(const std::string& src)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
	return cvt.from_bytes(src);
}

inline std::string wstring2u8string(const std::wstring& src)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
	return cvt.to_bytes(src);
}

inline std::string string2u8string(const std::string& src)
{
	return wstring2u8string(string2wstring(src));
}

inline std::string u8string2string(const std::string& src)
{
	return wstring2string(u8string2wstring(src));
}
}	//str_cvt
}	//libstr

#endif	//_STR_CVT_
