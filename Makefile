# 目标可执行文件名称, 如果是在非*nix下, 会自动加上.exe, 不必自行添加
TARGET := test

# 编译依赖的库文件, 没有可以留空
LIBS :=

# 自定义宏, 不须要自行添加 -D
DEFS +=	DEBUG	\

include ./Makefile.inc

