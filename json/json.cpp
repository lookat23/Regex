#include "StdAfx.h"
#include "json.h"
#include <string>

namespace haojson
{
// function
    void SkipSpace(const wchar_t*& c)
    {
        while(*c == L' ' || *c == L'\t' || *c == L'\r' || *c == L'\n' || *c == L'　')
        {
            c++;
        }
    }

    const wchar_t GetChar(const wchar_t*& c)
    {
        SkipSpace(c);
        return *c;
    }

    const wchar_t CharNext(const wchar_t*& c)
    {
        SkipSpace(c);
        const wchar_t ret = *c;
        if(*c != L'\0')
        {
            c++;
        }else
        {
            assert(false);
        }
        return ret;
    }

    void CharNextAndPromise(const wchar_t*& c, const wchar_t& OriginalChar)
    {
        assert( CharNext(c) == OriginalChar );
    }

    const wchar_t GetCharAndNext(const wchar_t*& c)
    {
        const wchar_t ret = GetChar(c);
        CharNext(c);

        return ret;
    }

	std::shared_ptr<IObject> GetObjectInstance(e_ObjectStatus status)
	{
		std::shared_ptr<IObject> ret;

		switch(status)
		{
		case osObject:
			{
				ret.reset(new CObjectObj);
			}
			break;
		case osObjectRight:
			{
				ret.reset(new CObjectRight);
			}
			break;
		case osObjectLeft:
			{
				ret.reset(new CObjectLeft);
			}
			break;
		case osArray:
			{
				ret.reset(new CObjectArray);
			}
			break;
		case osStr:
			{
				ret.reset(new CObjectStr);
			}
			break;
		case osNumber:
			{
				ret.reset(new CObjectNumber);
			}
			break;
		default:
			assert(false);
			break;
		}
		return ret;
	}

	std::shared_ptr<CObjectObj> GetObjectObjInstance()
	{ 
		return std::tr1::static_pointer_cast<CObjectObj>( GetObjectInstance(osObject) );
	}

	std::shared_ptr<CObjectRight> GetObjectRightInstance()
	{
		return std::tr1::static_pointer_cast<CObjectRight>( GetObjectInstance(osObjectRight) );
	}

	std::shared_ptr<CObjectLeft> GetObjectLeftInstance()
	{
		return std::tr1::static_pointer_cast<CObjectLeft>( GetObjectInstance(osObjectLeft) );
	}

	std::shared_ptr<CObjectArray> GetObjectArrayInstance()
	{
		return std::tr1::static_pointer_cast<CObjectArray>( GetObjectInstance(osArray) );
	}

	std::shared_ptr<CObjectStr> GetObjectStrInstance()
	{
		return std::tr1::static_pointer_cast<CObjectStr>( GetObjectInstance(osStr) );
	}

	std::shared_ptr<CObjectNumber> GetObjectNumberInstance()
	{
		return std::tr1::static_pointer_cast<CObjectNumber>( GetObjectInstance(osNumber) );
	}

// CObjectSubObj
    CObjectSubObj::CObjectSubObj()
    {

    }

    void CObjectSubObj::SetLeft(std::shared_ptr<CObjectLeft> obj)
    {
        m_objectLeft = obj;
    }

    std::shared_ptr<CObjectLeft> CObjectSubObj::GetLeft()
    {
        return m_objectLeft;
    }

    void CObjectSubObj::SetRight(std::shared_ptr<CObjectRight> obj)
    {
        m_objectRight = obj;
    }

    std::shared_ptr<CObjectRight> CObjectSubObj::GetRight()
    {
        return m_objectRight;
    }

// CObjectObj
	CObjectObj::CObjectObj()
	{

	}

	void CObjectObj::push_back( std::shared_ptr<CObjectSubObj> subObj )
	{
		m_vecSubObj.push_back(subObj);
	}

	std::shared_ptr<CObjectSubObj> CObjectObj::operator[](int i)
	{
		return m_vecSubObj[i];
	}

	size_t	CObjectObj::size()const
	{
		return m_vecSubObj.size();
	}

// CObjectLeft
	CObjectLeft::CObjectLeft()
		: m_obj(NULL)
	{

	}

	void CObjectLeft::SetObj(std::shared_ptr<IObject> obj)
	{
		assert(obj->GetObject() == osNumber || obj->GetObject() == osStr);
		m_obj = obj;
	}

	std::shared_ptr<IObject> CObjectLeft::GetObj()const
	{
		return m_obj;
	}

// CObjectRight
	CObjectRight::CObjectRight()
		: m_obj(NULL)
	{

	}


	void CObjectRight::SetObj(std::shared_ptr<IObject> obj)
	{
		m_obj = obj;
	}

	std::shared_ptr<IObject> CObjectRight::GetObj()const
	{
		return m_obj;
	}

// CObjectStr
	CObjectStr::CObjectStr()
	{

	}

	void CObjectStr::SetStr(const wchar_t* str)
	{
		m_str = str;
	}

	std::wstring CObjectStr::GetStr()const
	{
		return m_str;
	}

// CObjectNumber
	CObjectNumber::CObjectNumber()
		: m_IsDecimal(false)
	{

	}

	void CObjectNumber::SetNumber(const wchar_t* str, bool isDecimal)
	{
		m_str = str;
		m_IsDecimal = isDecimal;
	}

	std::wstring CObjectNumber::GetNumber()const
	{
		return m_str;
	}

// CObjectArray
	CObjectArray::CObjectArray()
	{

	}

	void CObjectArray::push_back(std::shared_ptr<CObjectRight> obj)
	{
		m_vecObj.push_back( obj );
	}

	size_t CObjectArray::size()const
	{
		return m_vecObj.size();
	}

	std::shared_ptr<CObjectRight> CObjectArray::operator[](int i)
	{
		return m_vecObj[i];
	}


// CJsonParse
    CNode CJsonParse::Parse( const wchar_t* str )
    {
        assert( str );

		CNode ret;
		std::shared_ptr<IObject> obj;

        switch(GetChar(str))
        {
        case L'{':
            {
                obj = ParseObject(str);
            }
            break;
        case L'[':
            {
                obj = ParseArray(str);
            }
            break;
		default:
			assert(false);
			break;
        }

		ret.SetObj( obj );

		return ret;
    }

    std::shared_ptr<CObjectObj> CJsonParse::ParseObject( const wchar_t*& str )
    {
        CharNextAndPromise(str, L'{');
		
		auto ret = GetObjectObjInstance();

        while(true)
        {
			auto subObj = std::make_shared<CObjectSubObj>();

            subObj->SetLeft( ParseObjectLeft(str) );
            assert( GetCharAndNext(str) == L':' );
            subObj->SetRight( ParseObjectRight(str) );
			ret->push_back(subObj);
            {
                const wchar_t c = GetCharAndNext(str);
                if(c == L'}')
                {
                    break;
                }else if( c != L',')
                {
                    assert(false);
                }
            }
        }

		return ret;
    }

    std::shared_ptr<CObjectLeft> CJsonParse::ParseObjectLeft(const wchar_t*& str)
    {
		auto ret = GetObjectLeftInstance();
		std::shared_ptr<IObject> obj;

        switch(GetChar(str))
        {
        case L'\"':
            {
                 obj = ParseStr(str);
            }
        break;
        case L'0':case L'1':case L'2':case L'3':case L'4':case L'5':case L'6':case L'7':case L'8':case L'9':
        {
            obj = ParseNumber(str);
        }
        break;
        default:
            assert(false);
        break;
        }

		ret->SetObj(obj);
		return ret;
    }

    std::shared_ptr<CObjectRight> CJsonParse::ParseObjectRight(const wchar_t*& str)
    {
		auto ret = GetObjectRightInstance();
		std::shared_ptr<IObject> obj;

        switch(GetChar(str))
        {
        case L'{':
            {
                obj = ParseObject(str);
            }
            break;
        case L'[':
            {
                obj = ParseArray(str);
            }
            break;
        case L'\"':
            {
                obj = ParseStr(str);
            }
            break;
        case L'0':case L'1':case L'2':case L'3':case L'4':case L'5':case L'6':case L'7':case L'8':case L'9':
            {
                obj = ParseNumber(str);
            }
            break;
        }
		ret->SetObj( obj );
		return ret;

    }

    std::shared_ptr<CObjectArray> CJsonParse::ParseArray( const wchar_t*& str )
    {
		auto ret = GetObjectArrayInstance();
		
        CharNextAndPromise(str, L'[');
        while(true)
        {
            auto obj = ParseObjectRight(str);
			ret->push_back( obj );

            const wchar_t c = GetCharAndNext(str);
            if(c == L']')
            {
                break;
            }
            assert(c == L',');
        }
        
		return ret;
    }

    std::shared_ptr<CObjectStr> CJsonParse::ParseStr( const wchar_t*& str )
    {
		auto ret = GetObjectStrInstance();

        CharNextAndPromise(str, L'\"');
        std::wstring name;

        while(true)
        {
            wchar_t c = *str;
            str++;

            if(c == L'\"' ) { break; }
            if(c == L'\\') { c = *(str++); }    // 有\号就取\后面的字符

            name.append(1, c);
        }
		ret->SetStr( name.c_str() );
		return ret;
    }

    std::shared_ptr<CObjectNumber> CJsonParse::ParseNumber( const wchar_t*& str )
    {
		auto ret = GetObjectNumberInstance();

        std::wstring name;

        bool IsDecimal = false; // 小数

        while(true)
        {
            const wchar_t c=  GetChar(str);
            
            if(c == L':' || c == L',' || c == L'}' || c == L']')
            {
                break;
            }else if(c == L'.')
            {
                if(IsDecimal)
                {
                    assert(false);  // 不可能有两个小数点
                }else
                {
                    IsDecimal = true;
                }
            }else if(c < L'0' || c > L'9')
            {
                assert(false);
            }

            name.append(1, GetChar(str));

            CharNext(str);
        }
		
		ret->SetNumber(name.c_str() , IsDecimal);

		return ret;
    }
}

