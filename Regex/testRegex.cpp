#include "stdafx.h"
#include "common/unittest/UnitTest.h"
#include "Regex.h"

void RegexAssert(const wchar_t* input, RegexNode node)
{
    Expression exp = ParseExpression(input);
    TEST_ASSERT( exp.IsEqual( node ) );
}

TEST_CASE(testRegex)
{
    RegexAssert(L"a", rC(L'a'));
    RegexAssert(L"hao", rC(L'h')+rC(L'a')+rC(L'o'));
}
