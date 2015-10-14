#pragma once
#include <string>

namespace haojson
{
//----------------------------------------------------------------------------------------------
// Exception
    class Exception
    {
    public:
        Exception(const std::wstring& _message=std::wstring());
        virtual ~Exception(){};
    protected:
        std::wstring        message;
    };

}