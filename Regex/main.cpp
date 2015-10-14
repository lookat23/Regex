#include "stdafx.h"
#include "common/unittest/UnitTest.h"

int _tmain(int argc, _TCHAR* argv[])
{
    int tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );
    tmpFlag |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag( tmpFlag );

    //TEST_RUN(testjson); 

    TEST_RUN_ALL();

	//system("pause");
    //_CrtDumpMemoryLeaks();
    return 0;
}

