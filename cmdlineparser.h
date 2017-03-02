/*
    一个轻巧的命令行解析工具类，只有一个头文件

    将命令行按分解为key-value的参数组，指定key值获得value，或通过paramPairsCount()获得参数组数后遍历获得所有key-value
    使用标准C++/STL实现，支持VC/GCC
                  by caijw 2014/10/30

    用例：
         ccutils::cmdline::CmdLineParser parser;
         parser.parse("test.exe --a=0 -b=-1999 c=9999999999 --c=3.14159265 --c=\"hello world\"");
         parser.get...();

         getCmdLine() = "test --a=0 -b=-1999 c=9999999999 --c=3.14159265 --c=hello world"
         argc() = 6
         argv(0) = "test"
         argv(1) = "--a=0"
         argv(2) = "-b=-1999"
         argv(3) = "c=9999999999"
         argv(4) = "--c=3.14159265"
         argv(5) = "--c=hello world"

         paramPairsCount() = 6
         getKey(0) = "", getValStr(0) = "test", getValInt(0) = 0, getValUInt(0) = 0, getValInt64(0) = 0, getValUInt64(0) = 0, getValFloat(0) = 0, getValDouble(0) = 0
         getKey(1) = "a", getValStr(1) = "0", getValInt(1) = 0, getValUInt(1) = 0, getValInt64(1) = 0, getValUInt64(1) = 0, getValFloat(1) = 0, getValDouble(1) = 0
         getKey(2) = "b", getValStr(2) = "-1999", getValInt(2) = -1999, getValUInt(2) = 4294965297, getValInt64(2) = -1999, getValUInt64(2) = 18446744073709549617, getValFloat(2) = -1999, getValDouble(2) = -1999
         getKey(3) = "c", getValStr(3) = "9999999999", getValInt(3) = 2147483647, getValUInt(3) = 4294967295, getValInt64(3) = 9999999999, getValUInt64(3) = 9999999999, getValFloat(3) = 1e+10, getValDouble(3) = 1e+10
         getKey(4) = "c", getValStr(4) = "3.14159265", getValInt(4) = 3, getValUInt(4) = 3, getValInt64(4) = 3, getValUInt64(4) = 3, getValFloat(4) = 3.14159, getValDouble(4) = 3.14159
         getKey(5) = "c", getValStr(5) = "hello world", getValInt(5) = 0, getValUInt(5) = 0, getValInt64(5) = 0, getValUInt64(5) = 0, getValFloat(5) = 0, getValDouble(5) = 0

         getParamKeys() = ["a", "b", "c"]
         getValStr("a") = "0", getValInt("a") = 0, getValUInt("a") = 0, getValInt64("a") = 0, getValUInt64("a") = 0, getValFloat("a") = 0, getValDouble("a") = 0
         getValStr("b") = "-1999", getValInt("b") = -1999, getValUInt("b") = 4294965297, getValInt64("b") = -1999, getValUInt64("b") = 18446744073709549617, getValFloat("b") = -1999, getValDouble("b") = -1999
         getValStr("c", 0) = "9999999999", getValInt("c", 0) = 2147483647, getValUInt("c", 0) = 4294967295, getValInt64("c", 0) = 9999999999, getValUInt64("c", 0) = 9999999999, getValFloat("c", 0) = 1e+10, getValDouble("c", 0) = 1e+10
         getValStr("c", 1) = "3.14159265", getValInt("c", 1) = 3, getValUInt("c", 1) = 3, getValInt64("c", 1) = 3, getValUInt64("c", 1) = 3, getValFloat("c", 1) = 3.14159, getValDouble("c", 1) = 3.14159
         getValStr("c", 2) = "hello world", getValInt("c", 2) = 0, getValUInt("c", 2) = 0, getValInt64("c", 2) = 0, getValUInt64("c", 2) = 0, getValFloat("c", 2) = 0, getValDouble("c", 2) = 0
*/
#pragma once

#include <string>
#include <string.h>
#include <sstream>
#include <vector>
#include <set>
#include <algorithm>

namespace ccutils
{
    namespace cmdline
    {
        class CmdLineParser
        {
        private:
            struct ParamInfo
            {
                ParamInfo(const std::wstring &k = std::wstring(), const std::wstring &v = std::wstring())
                {
                    assign(k, v);
                }

                ParamInfo& assign(const std::wstring &k, const std::wstring &v)
                {
                    key = k;
                    value = v;
                    return *this;
                }

                ParamInfo& assign(const std::wstring& assignExp, bool keyPrior)
                {
                    clear();
                    WStringList kv = split(assignExp, '=');
                    if (kv.size() == 2)
                    {
                        assign(kv[0], kv[1]);
                    }
                    else
                    {
                        keyPrior ? key = assignExp : value = assignExp;
                    }

                    return *this;
                }

                void clear()
                {
                    key.clear();
                    value.clear();
                }

                bool empty() const
                {
                    return key.empty() && value.empty();
                }

                std::wstring key;
                std::wstring value;
            };

            struct KeyMatcher
            {
                std::wstring key;
                int machTimes;
                int i;

                // t = -1 默认，只要key相等就算匹配
                // t >= 0 支持指定匹配的序号，第N个key相等才算真正匹配
                KeyMatcher(const std::wstring& k, int t = -1) : key(k), machTimes(t), i(0) {}
                bool operator()(const ParamInfo& param)
                {
                    if (!key.empty() && param.key == key)
                    {
                        return machTimes == -1 || i++ == machTimes;
                    }
                    return false;
                }
            };

            struct ValueMatcher
            {
                std::wstring value;

                ValueMatcher(const std::wstring& v) : value(v) {}
                bool operator()(const ParamInfo& param)
                {
                    return !value.empty() && param.value == value;
                }
            };

            struct ParamMatcher
            {
                std::wstring val;

                ParamMatcher(const std::wstring& v) : val(v) {}
                bool operator()(const ParamInfo& param)
                {
                    return !val.empty() && (param.key == val || param.value == val);
                }
            };

            typedef std::vector<std::wstring> WStringList;

        public:
            explicit CmdLineParser(const std::wstring& cmdLine = std::wstring())
            {
                if (!cmdLine.empty())
                {
                    parse(cmdLine);
                }
            }

            CmdLineParser(int argc, char* argcv[])
            {
                parse(argc, argcv);
            }

            // 解析命令行
            // 如果解析发现错误（如双引号未关闭），返回false
            bool parse(const std::wstring& cmdLine)
            {
                m_cmdLine = cmdLine;
                m_args.clear();

                wchar_t ch;
                std::wstring buf;
                buf.reserve(64);
                bool inQuote = false;
                for (unsigned i = 0; i < cmdLine.size(); ++i)
                {
                    ch = cmdLine[i];

                    if (ch == '\"')
                    {
                        inQuote = !inQuote;
                        continue;
                    }

                    if ((ch == ' ' || ch == '\t') && !inQuote)
                    {
                        if (!buf.empty())
                        {
                            m_args.push_back(buf);
                            buf.clear();
                        }
                        continue;
                    }

                    buf.push_back(ch);
                }

                if (!buf.empty())
                {
                    m_args.push_back(buf);
                }

                parseArgs();

                return !inQuote;
            }

            // 解析命令行
            // 内部使用UNICODE，将对argcv进行转换，调用前务必设置正确 setlocale(LC_CTYPE, "");
            void parse(int argc, char* argcv[])
            {
                m_cmdLine.clear();
                m_args.clear();
                for (int i = 0; i < argc; ++i)
                {
                    size_t argSize = strlen(argcv[i]) + 1;
                    wchar_t* warg = new wchar_t[argSize];
    #ifdef WIN32
                    size_t wargSize = 0;
                    mbstowcs_s(&wargSize, warg, argSize, argcv[i], _TRUNCATE);
    #else
                    mbstowcs(warg, argcv[i], argSize);
    #endif // WIN32
                    m_args.push_back(warg);

                    if (i != 0) m_cmdLine.append(L" ");
                    m_cmdLine.append(warg);

                    delete[] warg;
                }

                parseArgs();
            }

            void parse(int argc, wchar_t* wargcv[])
            {
                m_args.clear();
                for (int i = 0; i < argc; ++i)
                {
                    m_args.push_back(wargcv[i]);
                }
                parseArgs();
            }

            std::wstring getCmdLine() const
            {
                return m_cmdLine;
            }

            unsigned getArgc() const
            {
                return m_args.size();
            }

            std::wstring getArgv(unsigned i) const
            {
                if (i < m_args.size())
                {
                    return m_args[i];
                }

                return std::wstring();
            }

            std::set<std::wstring> getParamKeys() const
            {
                std::set<std::wstring> outKeys;
                for (ParamContainer::const_iterator it = m_paramPairs.begin(); it != m_paramPairs.end(); ++it)
                {
                    if (!it->key.empty()) outKeys.insert(it->key);
                }
                return outKeys;
            }

            // 获得解析后的参数组数目
            unsigned getParamsCount() const
            {
                return m_paramPairs.size();
            }

            // 是否存在指定的键名或键值
            unsigned hasParam(const std::wstring &val) const
            {
                if (!val.empty())
                {
                    return std::count_if(m_paramPairs.begin(), m_paramPairs.end(), ParamMatcher(val));
                }

                return 0;
            }

            // 是否存在指定键名的参数，返回数目
            unsigned hasKey(const std::wstring &key) const
            {
                if (!key.empty())
                {
                    return std::count_if(m_paramPairs.begin(), m_paramPairs.end(), KeyMatcher(key));
                }

                return 0;
            }

            // 是否存在指定键值的参数，返回数目
            unsigned hasVal(const std::wstring &val) const
            {
                if (!val.empty())
                {
                    return std::count_if(m_paramPairs.begin(), m_paramPairs.end(), ValueMatcher(val));
                }

                return 0;
            }

            // 根据参数组序号查询键名
            // 如果不是规范格式的参数组，键名将返回空，如 "copy file1 file2"
            std::wstring getKeyName(unsigned idx) const
            {
                if (idx < m_paramPairs.size())
                {
                    return m_paramPairs[idx].key;
                }

                return std::wstring();
            }

            // 根据参数组序号查询键值
            // 如果没有指定，键值将为空，如 "bash --help"
            std::wstring getValStr(unsigned idx) const
            {
                if (idx < m_paramPairs.size())
                {
                    return m_paramPairs[idx].value;
                }

                return std::wstring();
            }

            // 根据键名获得键值，字符串形式
            // 如果 hasKey 表示存在多个相同键名，可以指定 valIdx 来补充指定要获取的项目
            std::wstring getValStr(const std::wstring &key, unsigned valIdx = 0) const
            {
                std::wstring finalKey = trimLeft(key);
                if (!finalKey.empty())
                {
                    ParamContainer::const_iterator itResult = std::find_if(m_paramPairs.begin(), m_paramPairs.end(), KeyMatcher(finalKey, valIdx));
                    if (itResult != m_paramPairs.end())
                    {
                        return itResult->value;
                    }
                }

                return std::wstring();
            }

            // 根据参数组序号查询键值
            int getValInt(unsigned idx) const
            {
                return str2number<int>(getValStr(idx));
            }

            // 根据键名获得键值
            int getValInt(const std::wstring &key, unsigned valIdx = 0) const
            {
                return str2number<int>(getValStr(key, valIdx));
            }

            // 根据参数组序号查询键值
            unsigned getValUInt(unsigned idx) const
            {
                return str2number<unsigned>(getValStr(idx));
            }

            // 根据键名获得键值
            unsigned getValUInt(const std::wstring &key, unsigned valIdx = 0) const
            {
                return str2number<unsigned>(getValStr(key, valIdx));
            }

            // 根据参数组序号查询键值
            long long getValInt64(unsigned idx) const
            {
                return str2number<long long>(getValStr(idx));
            }

            // 根据键名获得键值
            long long getValInt64(const std::wstring &key, unsigned valIdx = 0) const
            {
                return str2number<long long>(getValStr(key, valIdx));
            }

            // 根据参数组序号查询键值
            unsigned long long getValUInt64(unsigned idx) const
            {
                return str2number<unsigned long long>(getValStr(idx));
            }

            // 根据键名获得键值
            unsigned long long getValUInt64(const std::wstring &key, unsigned valIdx = 0) const
            {
                return str2number<unsigned long long>(getValStr(key, valIdx));
            }

            // 根据参数组序号查询键值
            float getValFloat(unsigned idx) const
            {
                return str2number<float>(getValStr(idx));
            }

            // 根据键名获得键值
            float getValFloat(const std::wstring &key, unsigned valIdx = 0) const
            {
                return str2number<float>(getValStr(key, valIdx));
            }

            // 根据参数组序号查询键值
            double getValDouble(unsigned idx) const
            {
                return str2number<double>(getValStr(idx));
            }

            // 根据键名获得键值
            double getValDouble(const std::wstring &key, unsigned valIdx = 0) const
            {
                return str2number<double>(getValStr(key, valIdx));
            }

        private:
            void parseArgs()
            {
                m_paramPairs.clear();

                ParamInfo buf;
                for (WStringList::const_iterator it = m_args.begin();
                    it != m_args.end();
                    ++it)
                {
                    if (it->find('-') == 0) // start with '-'
                    {
                        takeParam(buf);
                        buf.assign(trimLeft(*it), true);
                        if (!buf.value.empty())
                        {
                            // --xxx=yyy
                            takeParam(buf);
                        }
                        // else // --xxx yyy
                    }
                    else
                    {
                        if (buf.empty())
                        {
                            buf.assign(*it, false);
                            takeParam(buf);
                        }
                        else
                        {
                            buf.value = *it;
                            takeParam(buf);
                        }
                    }
                }
                takeParam(buf);
            }

            static WStringList split(const std::wstring src, const char sep)
            {
                WStringList outList;

                std::wstring::size_type prev_pos = 0, pos = 0;
                while ((pos = src.find(sep, pos)) != std::wstring::npos)
                {
                    std::wstring substring(src.substr(prev_pos, pos - prev_pos));
                    outList.push_back(substring);
                    prev_pos = ++pos;
                }

                outList.push_back(src.substr(prev_pos, pos - prev_pos)); // Last word

                return outList;
            }

            static std::wstring trimLeft(const std::wstring& src, wchar_t ch = '-')
            {
                int idx = src.find_first_not_of(ch);
                if (idx != -1)
                {
                    return src.substr(idx);
                }

                return src;
            }

            void takeParam(ParamInfo &kv)
            {
                if (!kv.empty())
                {
                    m_paramPairs.push_back(kv);
                    kv.clear();
                }
            }

            template<typename T>
            static T str2number(const std::wstring &str)
            {
                T out = 0;
                std::wistringstream ws(str);
                ws >> out;
                return out;
            }

        private:
            typedef std::vector<ParamInfo> ParamContainer;
            std::wstring    m_cmdLine;
            WStringList     m_args;
            ParamContainer  m_paramPairs;
        };
    };
};
