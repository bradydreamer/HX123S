本文件夹下有四个工程，分别为：

1.sdktest:只包含测试代码，通过静态链接libsdk.a实现sdk的测试，用于libsdk进行测试。
2.libsdk:libsdk.a的编译工程，生成libsdk.a目标，用于发布libsdk。
3.K21App:测试程序和sdk程序整体编译工程，方便sdk组成员调试使用。
4.G2libdevtest：libsdk中针对dev就ddi成接口的封装测试，方便sdk组成员测试dev接口使用。