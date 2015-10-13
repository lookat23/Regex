#pragma once

class RegexNode
{
public:
    RegexNode()
    {

    }
    ~RegexNode()
    {

    }
    void SetNodeStr(wchar_t c)
    {
        wchar_t tmp[2] = {0};
        tmp[0] = c;
        nodeStr = tmp;
    }
    std::wstring GetNodeStr()const
    {
        return nodeStr;
    }

    RegexNode& operator+(const RegexNode& regexnode)
    {
        nodeStr += regexnode.GetNodeStr();

        return *this;
    }
private:
    std::wstring nodeStr;
};

class Expression
{
public:
    void Parse(const wchar_t* s)
    {
        str = s;
    }

    bool IsEqual(RegexNode& node)
    {
        return str == node.GetNodeStr();
    }
private:
    std::wstring str;
};




RegexNode rC(wchar_t c)
{
    RegexNode rn;
    rn.SetNodeStr(c);

    return rn;
}

Expression ParseExpression(const wchar_t* input)
{
    Expression e;
    e.Parse(input);
    return e;
}