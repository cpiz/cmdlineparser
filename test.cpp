#include "cmdlineparser.h"
#include <iostream>
#include <locale.h>

void test();
void stdOutput(const ccutils::cmdline::CmdLineParser &parser);

#ifdef WIN32
int wmain(int argc, wchar_t* argcv[])
#else
int main(int argc, char* argcv[])
#endif
{
    setlocale(LC_CTYPE, "");                // MinGW gcc.
    std::ios::sync_with_stdio(false);       // Linux gcc.
#ifndef CYGWIN
    std::locale::global(std::locale(""));
    std::wcout.imbue(std::locale(""));      // Mac
#endif

    if (argc > 1)
    {
        ccutils::cmdline::CmdLineParser parser;
        parser.parse(argc, argcv);
        stdOutput(parser);
    }
    else
    {
        test();
    }

    return EXIT_SUCCESS;
}

void test()
{
    std::vector<std::wstring> testcmdlines;
    testcmdlines.push_back(L"");
    testcmdlines.push_back(L"copy.exe");
    testcmdlines.push_back(L"copy.exe a");
    testcmdlines.push_back(L"copy.exe \"中文\" b");
    testcmdlines.push_back(L"copy.exe --src=srcfile --des=desfile");
    testcmdlines.push_back(L"copy.exe --src srcfile --des desfile");
    testcmdlines.push_back(L"copy.exe -src srcfile -des desfile");
    testcmdlines.push_back(L"copy.exe src=xxx");
    testcmdlines.push_back(L"%systemroot%\\system32\\csrss.exe objectdirectory=\\windows sharedsection=1024,20480,768 windows=on subsystemtype=windows serverdll=basesrv,1 serverdll=winsrv:userserverdllinitialization,3 serverdll=winsrv:conserverdllinitialization,2 serverdll=sxssrv,4 profilecontrol=off maxrequestthreads=16");
    testcmdlines.push_back(L"\"c:\\program files\\microsoft office\\office15\\winword.exe\" /n \"c:\\users\\caijw\\appdata\\roaming\\foxmail7\\temp-2992\\yy教育-新客户端11.05版本需求说明书-节日运营-v1.0.doc");

    for (unsigned i = 0; i < testcmdlines.size(); ++i)
    {
        std::wcout << "-------------------------------------------------------------------------------" << std::endl;
        ccutils::cmdline::CmdLineParser parser;
        parser.parse(testcmdlines[i]);
        stdOutput(parser);
    }
}

void stdOutput(const ccutils::cmdline::CmdLineParser &parser)
{
    std::wcout << L"getCmdLine() = \"" << parser.getCmdLine() << L"\"" << std::endl;
    std::wcout << std::endl;

    std::wcout << L"argc() = " << parser.getArgc() << std::endl;
    for (unsigned i = 0; i < parser.getArgc(); ++i)
    {
        std::wcout << L"argv(" << i << ") = \"" << parser.getArgv(i) << "\"" << std::endl;
    }
    std::wcout << std::endl;

    std::wcout << L"paramPairsCount() = " << parser.getParamsCount() << std::endl;
    for (unsigned i = 0; i < parser.getParamsCount(); ++i)
    {
        std::wcout
            << "getKey(" << i << ") = \"" << parser.getKeyName(i) << "\""
            << ", getValStr(" << i << ") = \"" << parser.getValStr(i) << "\""
            << ", getValInt(" << i << ") = " << parser.getValInt(i)
            << ", getValUInt(" << i << ") = " << parser.getValUInt(i)
            << ", getValInt64(" << i << ") = " << parser.getValInt64(i)
            << ", getValUInt64(" << i << ") = " << parser.getValUInt64(i)
            << ", getValFloat(" << i << ") = " << parser.getValFloat(i)
            << ", getValDouble(" << i << ") = " << parser.getValDouble(i)
            << std::endl;
    }
    std::wcout << std::endl;

    std::set<std::wstring> keys = parser.getParamKeys();
    std::wcout << "getParamKeys() = [";
    for (std::set<std::wstring>::const_iterator it = keys.begin(); it != keys.end(); ++it)
    {
        if (it != keys.begin())
            std::wcout << ", ";
        std::wcout << "\"" << *it << "\"";
    }
    std::wcout << "]" << std::endl;
    for (std::set<std::wstring>::const_iterator it = keys.begin(); it != keys.end(); ++it)
    {
        unsigned keyCount = parser.hasKey(*it);
        for (unsigned i = 0; i < keyCount; ++i)
        {
            std::wstringstream keyIdx;
            if (keyCount > 1) keyIdx << L", " << i;

            std::wcout
                << "getValStr(\"" << *it << "\"" << keyIdx.str() << ") = \"" << parser.getValStr(*it, i) << "\""
                << ", getValInt(\"" << *it << "\"" << keyIdx.str() << ") = " << parser.getValInt(*it, i)
                << ", getValUInt(\"" << *it << "\"" << keyIdx.str() << ") = " << parser.getValUInt(*it, i)
                << ", getValInt64(\"" << *it << "\"" << keyIdx.str() << ") = " << parser.getValInt64(*it, i)
                << ", getValUInt64(\"" << *it << "\"" << keyIdx.str() << ") = " << parser.getValUInt64(*it, i)
                << ", getValFloat(\"" << *it << "\"" << keyIdx.str() << ") = " << parser.getValFloat(*it, i)
                << ", getValDouble(\"" << *it << "\"" << keyIdx.str() << ") = " << parser.getValDouble(*it, i)
                << std::endl;
        }
    }
}
