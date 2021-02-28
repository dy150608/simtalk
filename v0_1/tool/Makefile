#主路径
MAIN=/home/li/document/codes/simple-chat/

#搜素所有后缀为.cpp的文件
ALL_SOURCES=$(shell find -name "*.cpp")
 
#将参数的文件名部分的.cpp后缀替换为.o后缀，并与./out组合为新路径
SOURCE_TO_OBJECT = $(MAIN)obj/$(subst .cpp,.o,$(notdir $(1)))

#从源路径取出文件，调用函数执行替换操作，替换完成后保存了所有目标文件路径
ALL_OBJECTS=$(foreach src,$(ALL_SOURCES),$(call SOURCE_TO_OBJECT,$(src)))
 
#定义多行变量，将依赖文件编译成目标文件
define CREATE_OBJECT_TARGET
$(call SOURCE_TO_OBJECT,$(1)) : $(1)
	g++ -c -o $$@ $$^ -std=c++11
endef
 
all: $(ALL_OBJECTS)
 
clean:
	-rm $(ALL_OBJECTS)
 
#将所有目标文件链接为目标程序
#$(TARGET):$(ALL_OBJECTS)
#	g++ -o $@ $^
 
#从源路径取出文件，调用宏包逐个编译为目标文件
$(foreach src,$(ALL_SOURCES),$(eval $(call CREATE_OBJECT_TARGET,$(src))))
