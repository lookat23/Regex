#pragma once
#include <vector>
#include <memory>

#include "JsonNode.h"
/******************************************************************************/
namespace haojson{

    enum e_ObjectStatus
    {
        osObject,
        osObjectSubObj,
		osObjectRight,
		osObjectLeft,
        osArray,
        osStr,
        osNumber,
    };



    class IObject{
	public:
        virtual e_ObjectStatus GetObject()const = 0;
    };

	class CObject : public IObject
	{
	};

	class CObjectLeft : public CObject
	{
	public:
		CObjectLeft();
		virtual e_ObjectStatus GetObject()const
		{
			return osObjectLeft;
		}
		void SetObj(std::shared_ptr<IObject> obj);
		std::shared_ptr<IObject> GetObj()const;
	private:
		std::shared_ptr<IObject>	m_obj;
	};

	class CObjectRight : public CObject
	{
	public:
		CObjectRight();
		virtual e_ObjectStatus GetObject()const
		{
			return osObjectRight;
		}
		void SetObj(std::shared_ptr<IObject> obj);
		std::shared_ptr<IObject> GetObj()const;
	private:
		std::shared_ptr<IObject>	m_obj;
	};

	class CObjectSubObj : public CObject
	{
    public:
        CObjectSubObj();

        virtual e_ObjectStatus GetObject()const
        {
            return osObjectSubObj;
        }

        void SetLeft(std::shared_ptr<CObjectLeft> obj);
        std::shared_ptr<CObjectLeft> GetLeft();
        void SetRight(std::shared_ptr<CObjectRight> obj);
        std::shared_ptr<CObjectRight> GetRight();

    private:
		std::shared_ptr<CObjectLeft> 		m_objectLeft;
		std::shared_ptr<CObjectRight>		m_objectRight;
	};


	class CObjectObj : public CObject
	{
	public:
		CObjectObj();
		virtual e_ObjectStatus GetObject()const
		{
			return osObject;
		}
		void push_back( std::shared_ptr<CObjectSubObj> subObj );

		std::shared_ptr<CObjectSubObj> operator[](int i);
		size_t	size()const;
	private:
		std::vector< std::shared_ptr<CObjectSubObj> >		m_vecSubObj;
	};


	class CObjectStr : public CObject
	{
	public:
		CObjectStr();
		virtual e_ObjectStatus GetObject()const
		{
			return osStr;
		}
		void SetStr(const wchar_t* str);
		std::wstring GetStr()const;
	private:
		std::wstring m_str;
	};

	class CObjectNumber : public CObject
	{
	public:
		CObjectNumber();
		virtual e_ObjectStatus GetObject()const
		{
			return osNumber;
		}

		void SetNumber(const wchar_t* str, bool isDecimal);
		std::wstring GetNumber()const;
	private:
		std::wstring m_str;
		bool m_IsDecimal;	// ÊÇ·ñ¸¡µãÊý
	};

	class CObjectArray : public CObject
	{
	public:
		CObjectArray();
		virtual e_ObjectStatus GetObject()const
		{
			return osArray;
		}

		void push_back(std::shared_ptr<CObjectRight> obj);
		size_t size()const;
		std::shared_ptr<CObjectRight> operator[](int i);
	private:
		std::vector< std::shared_ptr<CObjectRight> >	m_vecObj;
	};
	
    class CJsonParse
    {
    public:
        CNode Parse(const wchar_t* str);
    private:
        std::shared_ptr<CObjectObj> ParseObject(const wchar_t*& str);
        std::shared_ptr<CObjectLeft> ParseObjectLeft(const wchar_t*& str);
        std::shared_ptr<CObjectRight> ParseObjectRight(const wchar_t*& str);
        std::shared_ptr<CObjectArray> ParseArray(const wchar_t*& str);
        std::shared_ptr<CObjectStr> ParseStr(const wchar_t*& str);
        std::shared_ptr<CObjectNumber> ParseNumber(const wchar_t*& str);
    };
}
