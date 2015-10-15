#include "stdafx.h"
#include "common/unittest/UnitTest.h"
#include <sstream>
#include <functional>

class Exception
{
public:
    Exception(std::wstring message = std::wstring())
        : _message(message)
    {
    }
    std::wstring GetMessage()const
    {
        return _message;
    }
private:
    std::wstring _message;
};

#define WIDEN2(str) L#str
#define WIDEN(str)  WIDEN2(str)

class Assert
{
public:
    Assert(const std::wstring& expr)
        : SMART_ASSERT_A(*this), SMART_ASSERT_B(*this)
    {
        _err = L"Failed: ";
        _err += expr + L"\n";
    }
    ~Assert()
    {
        assert(_err.c_str() && false);
        throw Exception(_err);
    }

public:
    Assert& SMART_ASSERT_A;
    Assert& SMART_ASSERT_B;
    //whatever member functions
    template<typename T>
    Assert& print_current_val(T t,const wchar_t* e)
    {
        _err += e;
        _err += L" = ";
        std::wstringstream ss;
        ss << t;
        _err += ss.str();
        _err += L"\n";

        return *this;
    }
    Assert& print_context(const wchar_t* file, const wchar_t* line)
    {
        _err += L"file: ";
        _err += file;
        _err += L"  ";
        _err += L"line: ";
        _err += line;
        _err += L"\n";

        return *this;
    }
private:
    void operator=(const Assert&);
    std::wstring _err;
};

Assert make_assert(const std::wstring& expr)
{
    return Assert(expr);
}

#define SMART_ASSERT_A(x) SMART_ASSERT_OP(x,B)
#define SMART_ASSERT_B(x) SMART_ASSERT_OP(x,A)
#define SMART_ASSERT_OP(x,next) \
SMART_ASSERT_A.print_current_val((x),L#x).SMART_ASSERT_##next

#define SMART_ASSERT(expr) \
if( (expr) ) ; \
else make_assert(L#expr).print_context(WIDEN(__FILE__),WIDEN(__LINE__)).SMART_ASSERT_A

TEST_CASE(testException)
{
//     try
//     {
//         funC();
//     }
//     catch (Exception& e)
//     {
//         int a = 10;
//     }
    try
    {
        int c1 = 1;
        int c2 = 2;
        int c3 = 3;

        SMART_ASSERT(1==2)(c1)(c2)(c3);

//         int a = 10;
//         MACRO1(c1)(c2)(c3);
//         ENSURE(1 == 2);
    }
    catch(Exception& e)
    {
        std::wstring s = e.GetMessage();
        int a = 10;
    }
    


}