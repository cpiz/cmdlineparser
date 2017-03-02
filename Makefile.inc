########################################################
#
# 通用Makefile
# 	caijw	2009-5-15	创建
#
########################################################

ifneq ($(wildcard /bin/sh),)
	RM-F := rm -f
	FINAL_TARGET := $(TARGET)
	OS_TYPE := $(shell uname)
else
	RM-F := del
	FINAL_TARGET =$(TARGET).exe
	OS_TYPE := $(OS)
endif

# 针对环境的编译优化参数
ifeq "$(OS_TYPE)" "Windows_NT"
	OPTIMIZE_PARA := -march=prescott -O2 -pipe -fomit-frame-pointer
	DEFS += Windows_NT
	ifeq "$(PROCESSOR_ARCHITECTURE)" "x86"
		DEFS += X86
	endif
endif

ifeq "$(OS_TYPE)" "SunOS"
	OPTIMIZE_PARA := -O2
	DEFS += SOLARIS
endif

ifeq "$(OS_TYPE)" "AIX"
	OPTIMIZE_PARA := -O2
	DEFS += AIXV3 AIXV5
endif

ifeq "$(OS_TYPE)" "HP-UX"
	OPTIMIZE_PARA := -O2
	DEFS += HPUX
endif

ifeq "$(OS_TYPE)" "Linux"
	OPTIMIZE_PARA := -O2
	DEFS += LINUX
endif

# 增加对CYGWIN的定义
ifeq "$(findstring CYGWIN, $(OS_TYPE))" "CYGWIN"
	FINAL_TARGET =$(TARGET).exe
	DEFS += CYGWIN
endif

# 编译器及编译选项
#CC = g++
#CXX = g++
CFLAGS := -Wall -g -MMD
CFLAGS += $(OPTIMIZE_PARA)
CFLAGS += $(addprefix -D, $(DEFS))
CXXFLAGS := $(CFLAGS)

# 编译处理流程
SOURCE 					:= 	$(wildcard *.c) \
							$(wildcard *.cpp)
OBJS					:=	$(patsubst %.c, %.o, $(patsubst %.cpp, %.o, $(SOURCE)))
DEPS					:=	$(patsubst %.o, %.d, $(OBJS))
MISSING_DEPS			:=	$(filter-out $(wildcard $(DEPS)), $(DEPS))
MISSING_DEPS_SOURCES 	:=	$(wildcard $(patsubst %.d, %.c, $(MISSING_DEPS)) \
							$(patsubst %.d, %.pp, $(MISSING_DEPS)))

.PHONY : all depend objs clean clean_all rebuild

all : $(FINAL_TARGET)

depend : $(DEPS)

objs : $(OBJS)

clean :
	$(RM-F) *.o
	$(RM-F) *.d

clean_all : clean
	$(RM-F) $(FINAL_TARGET)

rebuild : clean_all all

ifneq ($(MISSING_DEPS),)
$(MISSING_DEPS) :
	$(RM-F) $(patsubst %.d, %.o, $@)
endif

-include $(DEPS)

$(FINAL_TARGET) : $(OBJS)
	$(CXX) -o $(FINAL_TARGET) $(OBJS) $(addprefix -l, $(LIBS))

