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

#define VARI(x)\
    [&]()->std::wstring {\
    std::wostringstream ss;\
    ss << L#x << L" = " << x;\
    return ss.str();\
}()\

std::wstring gSS;

void* Fun1(int t)
{
    gSS += VARI(t);

    return *Fun1;
}

#define MACRO2(x)\
{\
    gSS += VARI(x);\
}\

#define MACRO1(x)\
{\
    gSS += VARI(x);\
}MACRO2\

void fun1()
{
    int a = 10;
}

#define M1(x, y)\
{\
    x;\
    int b = 10;\
    gSS += VARI(y);\
}

#define M4 2




#define M2(x)\
    fun1();\
    M1(M4,x);\
    

class M33
{
public:
    M33(){
        int a = 10;
    };
    //    template<typename T>
    //     M33(T t)
    //     {
    //         M2(t);
    //     }

    template<typename T>
    M33 operator()(T t)
    {
        M2(t);
        return M33(*this);
    }
    M33(const M33& )
    {
        int a = 10;
    }
private:
    std::wstring message;
};

#define M3(x)\
    M33()(x)\

class ScopeGuard
{
public:
    ScopeGuard(std::function<void()> callback_fun)
    {
        _callback_fun = callback_fun;
    }
    ~ScopeGuard()
    {
        _callback_fun();
    }
private:
    std::function<void()> _callback_fun;
};

#define M5(x)\
{\
    std::wstring err;\
    err += L"Failed: " L#x L"\n";\
    err += L"File: "  WIDEN(__FILE__) L" Line: " WIDEN(__LINE__) L"\n";\
    ScopeGuard onExit([](){\
        assert(err.c_str() && false);\
        throw Exception(err);\
});\
}

#define ENSURE(x)\
    if(!(x))\
{\
    std::wstring err;\
    err += L"Failed: " L#x L"\n";\
    err += L"File: "  WIDEN(__FILE__) L" Line: " WIDEN(__LINE__);\
    assert(err.c_str() && false);\
    throw Exception(err);\
}

extern void funB();
void funA()
{
    funB();
}

void funB()
{
    int t1 = 10;
    int t2 = 20;
    throw Exception();

}

void funC()
{
    try
    {
        funA();
    }
    catch (Exception& e)
    {
        int a = 10;
    }

}

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

        //M1(M1(M1(NULL, c1), c2),c3);
        //M1(c1)(c2)(c3);
        {
            //M3()(c1)(c2);
        }
        M3(c1)(c2)(c3);

        int a = 10;
        MACRO1(c1)(c2)(c3);
        ENSURE(1 == 2);
    }
    catch(Exception& e)
    {
        std::wstring s = e.GetMessage();
        int a = 10;
    }
    


}