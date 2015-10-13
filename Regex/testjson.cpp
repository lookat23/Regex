#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <shlwapi.h>
#include <sstream> 
#include "common/unittest/UnitTest.h"
#include "json\json.h"

#pragma comment(lib, "Shlwapi.lib")

// 获取应用程序路径
static const wchar_t* GetModulePath()
{
    static wchar_t s_ModulePath[MAX_PATH] = {0};
    if (s_ModulePath[0] == L'\0')
    {
        ::GetModuleFileName(NULL, s_ModulePath, MAX_PATH);
        int Len = wcslen(s_ModulePath);
        wchar_t* Ptr = s_ModulePath + Len;
        while (Ptr != s_ModulePath)
        {
            if (*Ptr == '\\')
            {
                Ptr[1] = '\0';
                break;
            }
            Ptr--;
        }

        // 规范路径 防止那些 .. 之类的
        wchar_t szTmp[MAX_PATH] = {0};
        ::PathCanonicalize(szTmp, s_ModulePath);
        wcscpy_s(s_ModulePath, szTmp);
    }

    return s_ModulePath;
}

std::wstring FileToString(const wchar_t* path)
{
    assert(path);

    std::wifstream t(path);  
    t.imbue(std::locale("chs"));
    std::wstringstream buffer;  
    buffer << t.rdbuf();  
    return std::wstring(buffer.str());  
}

TEST_CASE(testjson)
{
	setlocale(LC_ALL,"chs");

    std::wstring path = GetModulePath();
    path += L"test\\testjson1.txt";

    std::wstring contents = FileToString(path.c_str());
  
    haojson::CJsonParse jsonParse;

    haojson::CNode node = jsonParse.Parse(contents.c_str());
	std::wstring jsonFormatStr = node.Format();
	wprintf(L"%s", jsonFormatStr.c_str());

    TEST_ASSERT( jsonFormatStr == L"[{\"城市\":\"北京\",\"面积\":16800,\"人口\":1600},{\"城市\":\"上海\",\"面积\":6400,\"人口\":1800}]" );
}

TEST_CASE(testjsonNode)
{
    setlocale(LC_ALL,"chs");

    std::wstring path = GetModulePath();
    path += L"test\\testjson2.txt";

    std::wstring contents = FileToString(path.c_str());

    haojson::CJsonParse jsonParse;
    haojson::CNode node = jsonParse.Parse(contents.c_str());
    
    {
        haojson::CNode subNode = node[L"List"];
        subNode = subNode[0];
        {
            subNode = subNode[L"FinancialCalendarData"];

            size_t len = subNode.size();
            TEST_ASSERT(len == 4);
            for(size_t i=0; i<len; i++)
            {
                haojson::CNode CalendarDataNode = subNode[i];
                if(i == 0)
                {
                    TEST_ASSERT( CalendarDataNode[L"Date"].asString() == L"2\"015-10-12" );
                    TEST_ASSERT( CalendarDataNode[L"Time"].asString() == L"14:45" );
                    TEST_ASSERT( CalendarDataNode[L"Weightiness"].asString() == L"2" );
                    TEST_ASSERT( CalendarDataNode[L"CountryCode"].asString() == L"10006" );
                    TEST_ASSERT( CalendarDataNode[L"CountryName"].asString() == L"法国" );
                    TEST_ASSERT( CalendarDataNode[L"Content"].asString() == L"8月经常帐(亿欧元)" );
                    TEST_ASSERT( CalendarDataNode[L"ColumnCode"].asString() == L"9047" );
                    TEST_ASSERT( CalendarDataNode[L"Previous"].asString() == L"-4" );
                    TEST_ASSERT( CalendarDataNode[L"Predict"].asString() == L"" );
                    TEST_ASSERT( CalendarDataNode[L"CurrentValue"].asString() == L"+2" );
                    TEST_ASSERT( CalendarDataNode[L"Revised"].asString() == L"" );
                    TEST_ASSERT( CalendarDataNode[L"DataType"].asString() == L"3018" );
                    TEST_ASSERT( CalendarDataNode[L"DataTypeName"].asString() == L"贸易" );
                }else if(i == 3)
                {
                    TEST_ASSERT( CalendarDataNode[L"Date"].asString() == L"2015-10-12" );
                    TEST_ASSERT( CalendarDataNode[L"Time"].asString() == L"17:31" );
                    TEST_ASSERT( CalendarDataNode[L"Weightiness"].asString() == L"2" );
                    TEST_ASSERT( CalendarDataNode[L"CountryCode"].asString() == L"10003" );
                    TEST_ASSERT( CalendarDataNode[L"CountryName"].asString() == L"中国" );
                    TEST_ASSERT( CalendarDataNode[L"Content"].asString() == L"9月实际外商直接投资(亿美元)" );
                    TEST_ASSERT( CalendarDataNode[L"ColumnCode"].asString() == L"4088" );
                    TEST_ASSERT( CalendarDataNode[L"Previous"].asString() == L"87.1" );
                    TEST_ASSERT( CalendarDataNode[L"Predict"].asString() == L"" );
                    TEST_ASSERT( CalendarDataNode[L"CurrentValue"].asString() == L"95.6" );
                    TEST_ASSERT( CalendarDataNode[L"Revised"].asString() == L"" );
                    TEST_ASSERT( CalendarDataNode[L"DataType"].asString() == L"3025" );
                    TEST_ASSERT( CalendarDataNode[L"DataTypeName"].asString() == L"资本市场" );
                }
            }
        }

        {
            subNode = node[L"List"];
            subNode = subNode[0];
            subNode = subNode[L"HolidayReport"];

            size_t len = subNode.size();
            TEST_ASSERT(len == 3);
            for(size_t i=0; i<len; i++)
            {
                haojson::CNode CalendarDataNode = subNode[i];

                if(i==0)
                {
                    TEST_ASSERT( CalendarDataNode[L"FinancialDate"].asString() == L"2015-10-12" );
                    TEST_ASSERT( CalendarDataNode[L"FinancialEvent"].asString() == L"哥伦布日 假期休市" );
                }else if (i == 2)
                {
                    TEST_ASSERT( CalendarDataNode[L"FinancialTime"].asString() == L"" );
                    TEST_ASSERT( CalendarDataNode[L"FinancialEvent"].asString() == L"体育节 假期休市");
                }
            }
        }

    }
    
    
}