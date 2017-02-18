# CheckDuplicateFiles

## 简介
CheckForDuplicateFiles是一个基于MD5、SHA1、CRC32效验的重复文件检查工具，通过检查其文件的上述三个文件效验值，可知文件是否有重复，而不是同其他软件类似的简单检查。

主界面
![主界面](https://raw.githubusercontent.com/sakuraOnlin/CheckDuplicateFiles/master/doc/img/main.png)

主界面查找所有重复项
![主界面查找所有重复项](https://raw.githubusercontent.com/sakuraOnlin/CheckDuplicateFiles/master/doc/img/mainRepeat.png)

当前版本：0.1.19

## 特征
CheckDuplicateFiles使用C++ Qt编写的，依赖于 Qt，从而实现跨平台运行。它被设计为一个动态库和界面的方式，动态库提供文件效验，界面只需绘制相关结果即可。以保持动态库的复用以及在其上继续开发新的效验方式。
程序分为两部分：CheckFile动态库和GUI界面。

CheckFile：提供文件效验并返回值。

GUI：显示动态库计算的结果，并提供简单的文件检查重复的功能。


## 先决条件
### 获取到最新的源代码，并确保Qt版本符合要求
获取源代码
`git clone https://github.com/sakuraOnlin/CheckDuplicateFiles.git`

**注意：CheckFile动态库，Qt版本建议 Qt 5.0 以上，GUI界面需要Qt 5.7以上**

### 编译
打开Qt Creator并打开项目，编译并选择src运行。

## 二次开发
**在 examples 文件夹中有相关演示Demo**
### CheckFile 头文件简介

```bash
public:
    void setCheckType(int CheckType);
	#设置检查算法，详细请看 util/util.h 头文件简介
    bool setFilePath(QString filePath);
	#设置检查文件的完整路径
    void setUserFactore(Factory *userFacrory);
	#设置自定义检查模块工厂（二次开发新的文件检查算法）
    bool getOperatingStatus();
	#获取运行标志

signals:
    void signalFinalResult(util::ComputeResult result);
	#检查进度和结果信号
    void signalError(QString errStr);
	#检查错误，包含错误字符串
    void signalCalculationComplete();
	#检查完成，当整个文件的所有指定指定检查算法检查完成时发送此信号

public slots:
    void onStart();
	#开始检查
    void onStopCheck();
	#停止检查
    void onRestore();
	#重置

```

### util/util.h 头文件简介
```bash

    #检查类型
    enum CheckType
    {
        NOTYPE      = 0,
        MD5         = 1,
        SHA1        = 2,
        CRC32       = 4
    };

    #消息结果
    enum ResultMessageType
    {
        NoCheck    ,
            #未开始检查
        CheckIng   , 
            #检查中
        CheckOver  , 
            #检查结束
        CheckError , 
            #检查错误
        Warring
            #警告
    };

    #检查结果数据
    typedef struct ComputeResult
    {
        ResultMessageType   resultMessageType;
            #消息类型
        CheckType           checkHashType;
            #检查类型
        qint64              fileSize;
            #制定检查文件大小
        qint64              computeProgress;
            #检查进度
        QString             resultStr;
            #检查结果
        QString             filePath;
            #指定检查的文件路径
        QString             checkTypeName;
            #检查类型的可识别名称
    }CheckResult;

    #创建检查实例工厂数据
    typedef struct factoryCreateResult
    {
        Compute             *creatorComputr;
            #检查实例的内存地址
        CheckType           checkHashType;
            #检查类型
        QString             creatorErrStr;
            #当检查实例的地址为空时用于标明错误信息
    }FactoryResult;
```