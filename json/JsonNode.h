#pragma once
#include <string>
#include <memory>

namespace haojson
{
    class IObject;
    class CNode
    {
    public:
        std::wstring Format();
        void SetObj( std::shared_ptr<IObject> obj );

        CNode operator[](const wchar_t* id);

        CNode operator[](int id);

        std::wstring asString()const;

        size_t size()const;

    private:
        void Format(std::wstring& str, std::shared_ptr<IObject> obj );
    private:
        std::shared_ptr<IObject> m_obj;
    };

}

