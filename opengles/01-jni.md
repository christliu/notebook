# JNI

为了进一步熟悉手游打包流程，最近打算将移动平台的流程全都学一遍，包括Android的opengles3, iOS的metal，由于家里没有iMac，于是先从Android入手。

在网上找了Android教程，其中这一篇比较适合入门， https://www.jianshu.com/c/a25bf14495d7 。

先学习一下JNI（java native interface），即java本地化接口，可以在java当中调用C++的代码。

## 环境部署

为了学习java，特意地安装了eclipse，写了helloworld，这一步都很顺利。

最好在安装eclipse之前先下载好jdk，我电脑当中之前下载过AndroidStudio，其中带有jre，为了图方便我没有下载jdk，是直接把AndroidStudio当中的jre环境作为eclipse的Java环境，但是后面在编译出dll之后的调用出了问题。不好说是jdk的环境的问题还是代码的问题（代码确定有问题，copy的时候没有改函数名），于是我最终还是重新下载了jdk11. 

* jdk
* eclipse
* visual studio

## java代码编写

~~~java
package com.christliu;

public class Hello {
	
	public static native String getStringFromC();

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		System.out.println("Hello Java");
		System.out.println("c func result is "+getStringFromC());
	}
	static {
		System.loadLibrary("JniLibrary") ;
	}

}
~~~

先在eclipse当中新建java工程，新建java的类，在里面添加native接口，native是告诉java本接口是用c++实现的，需要在lib里面查找。

> 注意这里因为我用的jdk11，踩了一个坑，新建的class必须有package，否则会报错。这里我取名com.christliu

## 生成.h头文件

jni是用jdk提供的javah来生成.h文件的，具体方式为

1. 打开控制台，cd到com.christliu.Hello的顶级目录，即src这个目录下。（我没有尝试过直接到Hello.java这个目录下是否可行）。

2. 执行指令

   ~~~cpp
   javah com.christliu.Hello
   ~~~

3. 在src目录下会生成com_christliu_Hello.h头文件。

## 生成DLL

利用上一步生成的.h文件，在vs当中生成dll。

1. 打开vs，新建一个empty工程，修改工程为x64，修改Properties->General->Configuration Type为Dynamic Library.

2. 新.h文件添加到头文件当中。新建一个cpp文件，代码如下：

   ~~~cpp
   /*
   * Class:     com_zeno_jni_HelloJni
   * Method:    getStringFormC
   * Signature: ()Ljava/lang/String;
   */
   
   #include "jni.h"
   #include "com_christliu_Hello.h"
   
   JNIEXPORT jstring JNICALL Java_com_christliu_Hello_getStringFromC
   (JNIEnv *Env, jclass jclazz) {
   
   	return (*Env)->NewStringUTF(Env, "Jni C String");
   }
   ~~~

3. 找到jdk的include目录，我这里是C:\Program Files\Java\jdk-11.0.5\include。将这个目录，同其win32子目录，添加到include path当中。

4. 生成dll。

## 调用DLL

生成dll之后，有三种方法让java程序识别到。

1. 将其放到c:windows\system32目录下。
2. 将dll生成目录添加到环境变量当中，之后重启eclipse。
3. 在src同目录新建一个Lib目录，将dll文件拷贝到其中，在eclipse当中修改Java Build Path-> Libraries->Native Library location，添加Lib目录。执行即可。 