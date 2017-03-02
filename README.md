一个轻巧的命令行解析工具类，只有一个头文件

将命令行按分解为key-value的参数组，指定key值获得value，或通过paramPairsCount()获得参数组数后遍历获得所有key-value
使用标准C++/STL实现，支持VC/GCC
              by caijw 2014/10/30

用例：
```c++
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
```