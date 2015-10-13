#pragma once

// ��ֹ��������͵��������
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
private:\
    TypeName(const TypeName&); \
    void operator=(const TypeName&)

template<typename T, int N>
int ARRAYCOUNT(T (&arr)[N])
{
    return N;
}