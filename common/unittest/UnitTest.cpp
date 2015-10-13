// UnitTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "UnitTest.h"



std::map< std::string, cTestBase* >& GetmapTestObj()
{
    static std::map< std::string, cTestBase* > mapTestObj;
    return mapTestObj;
}