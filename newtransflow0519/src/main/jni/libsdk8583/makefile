#Copyright (c) 2005- XGD
#创 建 人: 石玮松    版    本: V0.01    日    期: 2012-09-14

###########################################################################################
#是否使用C++ 如果不使用直接 IsUseCPP=flase就可以了
IsUseCPP=flase


#如果您有自己的库要加入请在这里加入
#MYDLLS += -L$(POSDIR)/libdev/$(MACHINEMODE)/lib/ -lxgdmaths -lxgdmodule -ldev -lxgdemv
MYDLLS += -L $(sdkDir) -lsdkdev

#编译dll完成时会运行下面两个命令
#DllCMD1 = cp ./$(outfilename) -rf $(sdkDir)/$(outfilename)
#DllCMD2 = cp ./$(outfilename) -rf ./$(Folder).so

#编译exe完成时会运行下面两个命令
EXECMD1 = cp ./$(outfilename) ./p-exe-d.so
#EXECMD2 = cp ./$(outfilename) -rf $(POSDIR)/libdev/CnCupAPI/"libCnCupAPI"$(outfilename)


#----------------------------------------------------------------------#
#----------------------------------------------------------------------#
########################上面为配置区####################################
####################下面的代码不建议修改################################
#----------------------------------------------------------------------#
#----------------------------------------------------------------------#

#默认编译输出文件
outfilename=

#下面把所有文件都归加入了不用再手动加入文件
ALLSRC += $(wildcard ./*/*/*.c)
ALLSRC += $(wildcard ./*/*.c)
ALLSRC += $(wildcard ./*.c)



ifeq ($(OUT_TARGET),exe)
#   SRC += $(filter-out $(wildcard ./src/syntax/*.c),$(ALLSRC))
   SRC += $(ALLSRC)
#	SRC += $(wildcard ./src/test/*.c)
#	SRC += $(wildcard ./src/demo/*.c)
else
	tem += $(wildcard ./src/demo/*.c)
	tem += $(wildcard ./src/syntax/*.c)
	SRC += $(filter-out $(tem),$(ALLSRC))
 

endif
#Let the type char be unsigned, like unsigned char. -funsigned-char
ifeq ($(platform),PPlat)	
	CFLAGS += -Wno-pointer-sign
	CFLAGS +=  -Wfloat-equal 
endif

TARGET = ./$(outfilename)



#如果是exe文件 运行相应的命令
ifeq ($(OUT_TARGET),exe)
	CopyCMD1 = $(EXECMD1)
	CopyCMD2 = $(EXECMD2)
	CopyCMD3 = $(EXECMD3)
else
  	CopyCMD1 = $(DllCMD1)
	CopyCMD2 = $(DllCMD2)
	CopyCMD3 = $(DllCMD3)
endif



#驱动库头文件
CFLAGS += -I$(DevLibIncDir) 
#定义编译基础sdk的宏
CFLAGS +=  -D IS_BASE_SDK 


CFLAGS += -I$(sdkIncDir)

outfilename = 
outfilename2 =
outfilename3 =
outfilename4 =
ifeq ($(platform),PPlat)	
	outfilename2 = "p-"
else 
   ifeq ($(platform),KPlat)	 
		outfilename2 ="k-"
   else
		outfilename2 = "u-"
   endif
endif


ifeq ($(debugfalg),debug) 
	outfilename3 = "d"
else
	outfilename3 ="r"
endif



#动态库之外就是静态库
ifeq ($(OUT_TARGET),dll)
	outfilename4 = ".so"
else 
    outfilename4 = ".a"
endif

outfilename = $(Folder)"-"$(outfilename2)$(outfilename3)$(outfilename4)


#如果是可执行文件就把前面弄的名字盖掉
ifeq ($(OUT_TARGET),exe)
		ifeq ($(debugfalg),debug) 
			outfilename=$(MACHINEMODE)"-exe-d.so"
			else
			outfilename=$(MACHINEMODE)"-exe-r.so"    			
		endif	
#MYDLLS += -L$(POSDIR)/libdev/$(MACHINEMODE)/lib/ -lxgdmaths -lxgdmodule -ldev -lxgdemv 
#	MYDLLS += -L$(sdkDir)/DeviceLib/ -lsdkdev
else

#	MYDLLS +=-L$(POSDIR)/sdkLib/$(Sdk_Ver)/DeviceLib/ "-lDevice"
#	MYDLLS += -L$(POSDIR)/libdev/$(MACHINEMODE)/lib/ -lxgdmaths -lxgdmodule -ldev -lxgdemv 
#	MYDLLS += -L$(sdkDir)/DeviceLib/ -lsdkdev
endif


#如果使用C++就要加载C++库
ifeq ($(IsUseCPP),true)

#加上C++编译标志
 CFLAGS +=  -x c++
 
#K平台和P平台C++库所在的位置是不同的
	ifeq ($(platform),PPlat)		
	MYDLLS += -L ./opt/freescale/usr/local/gcc-4.1.2-glibc-2.5-nptl-3/arm-none-linux-gnueabi/arm-none-linux-gnueabi/lib/ -lstdc++
	
	else
	
	MYDLLS += -L ./usr/local/arm/3.4.1/arm-linux/lib/ -lstdc++
	
	endif 

endif


OBJS = $(patsubst %.c, %.o, $(SRC))

#MYLIBS = $(PROJECTDIR)/lib/libxgdkernel.a
#MYLIBS += $(PROJECTDIR)/lib/libappdevice.a

CFLAGS += -Os -s -fomit-frame-pointer -fno-strength-reduce 
#-Wall

#如果上次编译的是同一机型 那libdev下面的makefile就不用调用了
#ifeq ($(Last),error)   
#	dir_y += $(POSDIR)/libdev
#endif

SourceNameEnd=
ifeq ($(platform),PPlat) 
		ifeq ($(debugfalg),debug) 
		SourceNameEnd="-p-d.so"
		else
		SourceNameEnd="-p-r.so"
		endif	
else
		ifeq ($(debugfalg),debug) 
		SourceNameEnd="-k-d.so"
		else
		SourceNameEnd="-k-r.so"
		endif	
endif

SourceName=libsdkdev-$(MACHINEMODE)-$(outfilename3).so

all: aaa $(TARGET) 

aaa:
ifeq ($(platform),UCPlat)
else	
		cp $(sdkDir)/$(SourceName) -rf $(sdkDir)/libsdkdev.so
endif

#首先执行库 库里面的makefile生成与机器相送的两个头文件
	for i in  $(dir_y) ;do make -C $$i || exit$? ;done  

$(TARGET) : $(OBJS)
#	if [ $(MACHINEMODE) == K390 -o $(MACHINEMODE) == K820 ]; then \
#		MYDLLS += -lts; \
#	fi
ifeq ($(OUT_TARGET),slib)
	$(AR) -ruv  $(outfilename) $(OBJS)

else
	$(CC) -s -fPIC -lpthread $(LINTFLAG) -o $@ $(OBJS) -ldl -lm $(MYLIBS) $(MYDLLS) ;
endif	

	
ifeq ($(OUT_TARGET),dll)
	cp ./$(outfilename) -rf ./$(Folder)".so"
endif

	
%.o:%.c
	$(CC) $(CFLAGS) -o $@ -c $<
	
#$(SRC:.c=.o): ./inc/*.h $(INCDIR)/*.h
$(SRC:.c=.o):    $(sdkIncDir)/*.h
clean:
	-rm -f $(TARGET) $(SYNTAX) *.elf *.gdb *.o */*.o */*/*.o */*/*/*.o

