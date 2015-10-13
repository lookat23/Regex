#include "stdafx.h"
#include "JsonNode.h"
#include "json.h"
/******************************************************************************/
namespace haojson{
// AUX
    namespace{
        std::shared_ptr<CObjectObj> ConvertObjectObj(std::shared_ptr<IObject> obj)
        { 
            return std::tr1::static_pointer_cast<CObjectObj>( obj );
        }

        std::shared_ptr<CObjectRight> ConvertObjectRight(std::shared_ptr<IObject> obj)
        {
            return std::tr1::static_pointer_cast<CObjectRight>( obj );
        }

        std::shared_ptr<CObjectLeft> ConvertObjectLeft(std::shared_ptr<IObject> obj)
        {
            return std::tr1::static_pointer_cast<CObjectLeft>( obj );
        }

        std::shared_ptr<CObjectArray> ConvertObjectArray(std::shared_ptr<IObject> obj)
        {
            return std::tr1::static_pointer_cast<CObjectArray>( obj );
        }

        std::shared_ptr<CObjectStr> ConvertObjectStr(std::shared_ptr<IObject> obj)
        {
            return std::tr1::static_pointer_cast<CObjectStr>( obj );
        }

        std::shared_ptr<CObjectNumber> ConvertObjectNumber(std::shared_ptr<IObject> obj)
        {
            return std::tr1::static_pointer_cast<CObjectNumber>( obj );
        } 

        std::shared_ptr<CObjectSubObj> ConvertObjectSubObj(std::shared_ptr<IObject> obj)
        {
            return std::tr1::static_pointer_cast<CObjectSubObj>( obj );
        } 

        std::wstring asStringFun(std::shared_ptr<IObject> obj)
        {
            e_ObjectStatus statue = obj->GetObject();

            assert(statue == osNumber || statue == osStr || statue == osObjectSubObj || statue == osObjectRight);

            std::wstring ret;

            switch(statue)
            {
            case osNumber:
                {
                    ret = ConvertObjectNumber( obj )->GetNumber();
                }break;
            case osStr:
                {
                    ret =  ConvertObjectStr( obj )->GetStr();
                }break;
            case osObjectSubObj:
                {
                    ret =  asStringFun( ConvertObjectSubObj( obj )->GetRight() );
                }break;
            case osObjectRight:
                {
                    ret = asStringFun( ConvertObjectRight( obj )->GetObj() );
                }break;
            default:
                assert(false);
                break;
            }

            return ret;
        }
    }

    void CNode::SetObj( std::shared_ptr<IObject> obj )
    {
        m_obj = obj;
    }

    std::wstring CNode::Format()
    {
        std::wstring ret;
        Format(ret, m_obj);
        return ret;
    }

    void CNode::Format(std::wstring& str, std::shared_ptr<IObject> obj)
    {
        e_ObjectStatus status = obj->GetObject();
        switch(status)
        {
        case osObject:
            {
                auto tureObj = ConvertObjectObj(obj);
                str += L"{";
                for(size_t i=0; i<tureObj->size(); i++)
                {
                    Format(str, (*tureObj)[i]->GetLeft());
                    str += L":";
                    Format(str, (*tureObj)[i]->GetRight());
                    if(i != tureObj->size()-1) { str += L","; }
                }
                str += L"}";
            }
            break;
        case osObjectRight:
            {
                auto tureObj = ConvertObjectRight(obj);
                Format(str, tureObj->GetObj()); 
            }
            break;
        case osObjectLeft:
            {
                auto tureObj = ConvertObjectLeft(obj);
                Format(str, tureObj->GetObj());
            }
            break;
        case osArray:
            {
                auto tureObj = ConvertObjectArray(obj);
                str += L"[";
                for(size_t i=0; i<tureObj->size(); i++)
                {
                    Format(str, (*tureObj)[i]);
                    if(i != tureObj->size()-1) { str += L","; }
                }
                str += L"]";
            }
            break;
        case osStr:
            {
                auto tureObj = ConvertObjectStr(obj);
                str += L"\"";
                str += tureObj->GetStr();
                str += L"\"";
            }
            break;
        case osNumber:
            {
                auto tureObj = ConvertObjectNumber(obj);
                str += tureObj->GetNumber();
            }
            break;
        default:
            assert(false);
            break;
        }
    }

    haojson::CNode CNode::operator[]( const wchar_t* id )
    {
        e_ObjectStatus status = m_obj->GetObject();

        CNode ret;

        switch(status)
        {
        case osStr: case osNumber: case osArray:  // 铁定没有再下一级了
            {
                assert(false);
            }
            break;
        case osObjectRight: 
            {
                auto tureObj = ConvertObjectRight(m_obj);
                CNode SubNode;
                SubNode.SetObj(tureObj->GetObj());
                ret = SubNode[id];
            }
            break;
        case osObjectLeft:
            {
                assert(false);
            }
            break;
        case osObject:
            {
                auto tureObj = ConvertObjectObj(m_obj);
                size_t len = tureObj->size();
                for(size_t i=0; i<len; i++)
                {
                    e_ObjectStatus status = (*tureObj)[i]->GetLeft()->GetObj()->GetObject();
                    if(status == osNumber)
                    {
                        auto left = ConvertObjectNumber( (*tureObj)[i]->GetLeft()->GetObj() );
                        if( left->GetNumber() == id)
                        {
                            ret.SetObj( (*tureObj)[i]->GetRight() );
                            break;
                        }

                    }else if( status == osStr)
                    {
                        auto left = ConvertObjectStr( (*tureObj)[i]->GetLeft()->GetObj() );
                        if( left->GetStr() == id)
                        {
                            ret.SetObj( (*tureObj)[i]->GetRight() );
                            break;
                        }
                    }else
                    {
                        assert(false);
                    }
                }

            }
            break;
        default:
            assert(false);
        break;
        }
        return ret;
    }

    haojson::CNode CNode::operator[](int id)
    {
        e_ObjectStatus status = m_obj->GetObject();
        assert( status == osObjectRight);

        auto tureObj = ConvertObjectRight(m_obj);

        CNode ret;
        std::shared_ptr<IObject> obj;

        auto subObj = tureObj->GetObj();

        switch( subObj->GetObject() )
        {
        case osArray:
            {
                obj = (*ConvertObjectArray(subObj))[id];
            }break;
        case osObject:
            {
                obj =  (*ConvertObjectObj( subObj ))[id];
            }break;
        default:
            assert(false);
            break;
        }

        ret.SetObj( obj );

        return ret;
    }

    std::wstring CNode::asString()const
    {
        return asStringFun(m_obj);
    }

    size_t CNode::size()const
    {
        assert(osObjectRight == m_obj->GetObject());

        auto tureObj = ConvertObjectRight( m_obj )->GetObj();
        e_ObjectStatus status = tureObj->GetObject();
        assert( status == osArray || status  == osObject);

        size_t ret = 0;

        switch(status)
        {
        case osArray:
            {
                ret = ConvertObjectArray(tureObj)->size();
            }break;
        case osObject:
            {
                ret =  ConvertObjectObj( tureObj )->size();
            }break;
        default:
            assert(false);
            break;
        }

        return ret;
    }
}