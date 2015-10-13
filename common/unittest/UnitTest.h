#include <vector>
#include <string>
#include <map>

class cTestBase
{
public:
    cTestBase(){};
    virtual ~cTestBase(){};
    virtual void Run() = 0;
};

extern std::map< std::string, cTestBase* >& GetmapTestObj();

#define TEST_RUN_ALL()\
    {auto it = GetmapTestObj().begin();\
    for(; it!= GetmapTestObj().end(); it++)\
    {\
        it->second->Run();\
    }}

#define TEST_RUN(name)\
    {auto search = GetmapTestObj().find(#name);\
    if(search != GetmapTestObj().end())\
{\
    search->second->Run();\
}}

#define TEST_CASE(name)\
extern void testcase##name();\
class cTestCase##name : public cTestBase\
{\
public:\
    cTestCase##name(){ GetmapTestObj()[#name] = this; }\
    virtual void Run()\
{\
    testcase##name();\
}\
};\
static cTestCase##name cTestCase##name;\
void testcase##name()

#define TEST_ASSERT(x) do{ if(!(x)){throw;} }while(0);

