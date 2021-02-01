# 图书检测系统
包括程序源代码、历次的程序更改说明、需求分析、设计文档、参考文献。下周一上午到怀柔来一趟，把上述工作的电子版给我，另外看一下你的程序目前进展，以及文章和专利。

## 简介
**以下为大华相机中的示例中的说明，做了一些修改，仅供参考，不具有绝对参考价值**

1. ~~本例程用于演示接口的使用方法，基于QT5.5.0开发，使用之前请先安装QT5.5.0或以上版本。~~
   
    本工程使用的vs2015进行开发，使用的Qt版本为5.12.0.OpenCV版本为3.4.6。依赖于Sample/Qt/ShowQImage而改编

2. ~~本例程使用MV Viewer版本为2.1.2_Build20180323，使用之前请先安装2.1.2_Build20180323版本MV Viewer~~更新到了2.2.2

3. 本例程不包含修改相机的IP，使用之前请先用MV Viewer修改相机的IP到网卡所在的网段。

4. 目录结构如下
   
    关于软件位数问题，由于YOLO仅支持x64，所以软件的生成版本都只生成64位。大华SDK及sample中分x86和x64，x86的程序全部为未使用的，开发过程中可能会有x86参与，但无用。

	```
	/QMainWidget
		├ Include  ： 头文件
		│   ├ Media
		│   ├ GenICam     : SDK库的头文件。请配合所用MV Viewer版本里自带的头文件。(MV Viewer安装目录下\MV Viewer\Development\Include中)
		│   ├ Infra       ：SDK库的头文件。请配合所用MV Viewer版本里自带的头文件。(MV Viewer安装目录下\MV Viewer\Development\Include中)
		│   └ Memory      ：SDK库的头文件。请配合所用MV Viewer版本里自带的头文件。(MV Viewer安装目录下\MV Viewer\Development\Include中)
		│
		├ Depends  ：  编译时依赖库（大华相机中的lib）分为win32和x64，但是yolo只支持x64
		│   ├ ImageConvert.lib
		│   ├ VideoRender.lib  
		│   ├ yolo_cpp_dll.lib     :深度学习检测使用的lib  
		│   └ MVSDKmd.lib     ： SDK库。请配合所用MV Viewer版本里自带的头文件。(MV Viewer安装目录下\MV Viewer\Development\Lib中)
		│
		├ icon
		│   └ 窗体中快捷键的按钮，发布版本不能显示   
		│
		├ Src   ：程序源代码
		│   ├ mainwindow.h         ： 主界面头文件
		│   ├ mainwindow.cpp       ： 主界面实现
		│   ├ Config.h             ： 写入配置文件类头文件
		│   ├ Config.cpp           ： 写入配置文件类实现
		│   ├ takephoto.h          ： 拍照按钮弹窗
		│   ├ takephoto.cpp        ： 拍照按钮弹窗
		│   ├ alertwindow.h        ： 错误识别弹窗窗口
		│   ├ alertwindow.cpp      ： 错误识别弹窗窗口
		│   ├ MySerialport.h       ： 串口类头文件
		│   ├ MySerialport.cpp     ： 串口类实现
		│   ├ MessageQue.h         ： 帧缓存队列数据结构（必需！）
		│   ├ main.cpp             ： 主程序入口
		│   ├ parametersetting.h   ： 设置参数窗口
		│   └ parametersetting.cpp ： 设置参数窗口
		│
		├ Train     ： 图书训练存放照片和标记文件的目录	
		│   ├ Image
		│   └ Label
		│
		├ labelImg  ： 标记图书软件，由py文件生成的exe	
		│
   		├ TemplateEditor    ： 模板框定软件，exe文件
    	│
   		├ 所有的ui文件：alertwindow.ui、mainwindow.ui、parametersetting.ui
    	│
   		├ x64 依赖环境具体问题具体分析，调试时自己掌握
		│   ├ release
		│   │   ├ cudn
		│   │   ├ 大华相机,ImageConvert.dll用于图像格式转换，VideoRender.dll用来显示图像。
		│   │   ├ opencv
		│   │   ├ Qt
		│   │   └ yolo  
		│   └ debug
		│       ├ cudn
		│       ├ 大华相机,ImageConvert.dll用于图像格式转换，VideoRender.dll用来显示图像。
		│       ├ Qt
		│       ├ opencv
		│       └ yolo  
    	│
		└ QMainWidget.pro

	```
5. 接口使用时需要注意顺序。（这方面sample中已经设置好顺序，不需要做改动）

	1. 先发现相机（CameraCheck），再连接相机（CameraOpen），连接相机以后才可以修改曝光和增益、改变触发方式。
	2. 开始采集（CameraStart）以后才可以显示图像，否则缓存队列为空。

6. **注意事项。**
	1. 默认连接的是发现列表中的第一个相机。
	2. ShowImage接口没有获取图像功能（图像在回调函数中获取），只做显示用，需要给它传入一个指针pRgbFrameBuf，该指针指向要显示的图像数据。
	3. 目前工程只支持Mono8和RGB24图像数据转化成QImage，Mono8不需要转码可以直接生成QImage,其余格式通过ImageConvert库先转换成RGB24再生成QImage
	4. Sample中通过QLabel显示图像，QLabel只支持QPixmap。所以先将QImage转换为QPixmap，转换会造成图像显示延迟。
	5. 如何生成QImage请参照dahua.cpp中的ShowImage函数， 非mono8格式转码为RGB24请参照dahua.cpp中的DisplayThreadProc函数

<center>以上是自带的说明并加以修改</center>
<center> END </center>

----
# <center>以下为添加内容</center>

<br>



## Github地址
代码已经放置在github上进行托管，连接为 https://github.com/GTMHS/QtMainWidget

<br>

## 未实现或现存在的问题

1.  双击release下的exe文件不能打开，闪都不闪，直接没反应，这个问题一直时有时无。现在的问题是双击之后任务管理器中有相关的进程但是不弹窗。发布未尝试。

2. ~~识别时闪退。连接相机进行识别时，运行一会就会闪退；识别本地图片文件时，没有这种现象（在之前的命令行中。改为Qt之后没有试过）~~ 

    这个问题是改了识别过程中的内存释放的问题
```
    if (!(img->load(infile2))) //加载图像
    {
	    QMessageBox::information(this,tr("加载图像 FAILED"),tr("加载图像 FAILED!"));
	    delete img;
	    return;
    }
    ui->label->setPixmap(QPixmap::fromImage(*img));
    delete img;
```
3. 调用其他exe时闪退。调用的exe包括TemplateEditor.exe（框定图书用，之前是画模板的那个）、labelImg.exe图书标记软件，关闭弹出的exe之后，主窗口闪退。

    闪退问题现在没有，有一个关键的问题是使用templateEditor框选图书的位置之后，返回的结果并不是图书的位置，会发生偏移。
    
    参考的解决方案：在显示图片的位置`label`固定框处一个位置，让用户自动调整，把图书放在蓝框中。

    **把调用其他exe程序封装成一个类？统一使用一个调用方法来调用？**

4. ~~执行其他的命令时闪退，图书拍照、识别参数。图书拍照采用的是设置了一个参数，点击按钮之后将参数改为true，拍照保存图片，并将参数改为false。有时候能用有时候就会闪退。相机采用的是连续拉流。~~

    现在相机打开之后就是用的硬件触发，拍照同样采用的是弹窗按钮，然后硬件触发之后会保存触发之后拍的照片。近期拍照功能未使用过。

    识别参数是用训练好的模型去识别当前这本书，然后将识别到的数量和拟合的直线的参数保存起来，同时传给类内属性参数

5. ~~labelImg.exe 打不开，闪退（SHELLEXECUTEINFO 不能打开命令行？）。之前是直接打开标记软件，可以打开，现在在py文件中添加了复制文件，就不能打开了。但是直接双击labelImg是可以打开的。

    最近此功能也未用过，不知具体情况。~~

    20210131测试labelImg在x64 Release下可以打开。


6. 软件大小自适应问题。屏幕窗口大小适宜问题，缩放屏幕图片大小调整问题。
7. 参数的自动拟合，自动修正功能。能够自动修正参数信息，提供给用户是否要自动修复参数。
8. 相机工作时在采用硬件触发的情况下，会出现拍照模糊的情况，需要联系技术支持询问具体原因。
9. 更改了yolo识别检测的方式之后，程序报错！不能使用`#define`下定义的函数，只能使用外部函数，尝试将相机采集的图片保存为文本然后去检测？整个识别时间从700ms提升至70-ms。

10. config下的配置文件会增大，不知具体原因，需要跟进。
    
    **问题未复现，暂未解决**

    是不是由于强行退出导致的读写冲突之类的？

11. 画完模板之后读取模板图像，得到的位置信息有偏差，未曾找到原因。（图像裁剪之后得到的图片的位置不对，需要跟进，重新计算下图像位置）
12. 模型训练的步骤还未加进去。以及后续的使用工作。
13. icon在打包安装的软件中不显示。是不是也涉及到web中的静态文件问题？
14. 配置文献现在使用的是两个，一个存储串口号，另一个保存识别相关信息。后期将串口号也写入配置文件中。

----
<br>

# <center>开发文档</center>

## 开发环境
* Windows 10 1909 
* visual studio 2015 专业版
* Qt 5.12
## 默认配置项

* 配置文件检查`Config/conf.ini`，存储串口信息
* 新书拍照保存的位置是在 `Train/image/Pic.bmp`
* 标记文件保存的位置是在 `Train/label/Pic.xml`
* 裁剪完之后配置文件会把书的位置信息保存起来，并将新书图片进行裁剪，方法`on_actionOpenCutWindow_triggered()`
* 新书训练的步骤：先拍照，照片保存，然后用裁剪工具裁剪，裁剪完之后复制，之后标记，标记文件复制。
  复制了之后文件移动，修改配置文件，训练。

## 设计思想
1. 使用templateEditor.exe标记出一本书的模板，然后去识别出各个黑块的位置和相对位置信息，以及拟合的k/b/s。
2. 实时监测。通过模型进行检测，得到检测结果

	2.1 如果结果中的`N,K,b,s`都正确，则此书装订正确

	2.2 如果结果中的`N,k,b,s`不正确，则需要进一步处理

	- 若`k,b,s`错误，=>**装订错误**
	- 若`N`小于**N**：正序、逆序两种方式进行遍历已经识别到的黑块，根据相对位置关系，查找保存的各块之间的相对位置关系，若缺少的位置的平均灰度值小于指定值，则正确，否则错误	
	- 若`N`大于**N**：=>**装订错误**


----
## 使用手册

### 写在前面
#### 配置文件
- 串口文件配置：确保文件存在及串口编号：根目录下的`config`文件
- 模板文件配置：确保文件存在`Config/conf.ini`


### 日常使用
    点击打开摄像头->开始识别，开始识别使用

### 不需要训练的新书

![0](Img/dailyuse.jpg)
1. 在新书训练菜单中点击新书拍照，图片保存在`Train\image\Pic.bmp`，打开，框选图书位置，右键导出，保存在根目录的moban.png。图书被裁剪。
2. 点击`识别参数`菜单会对裁剪之后的图书识别参数，并将参数信息保存在`release\config\conf.ini`下，并将参数传递给类变量（似乎并没有传递成功？）识别完成之后会弹窗提示识别完成。
3. 点击开始识别即可使用。

### 新书训练
新书训练按照新书训练主菜单中的顺序依次点击完成即可
![1](Img/1.png)


1. 新书拍照。把书放到传送带上，**摆正**，（添加一个仿射变换？）然后点击拍照。
2. 新书裁剪。新书裁剪菜单中先打开裁剪窗口，框选图书部位，然后导出模板，关闭窗口。之后再点击裁剪图片菜单，完成图片裁剪。
3. 新书标记。点击新书标记裁剪，打开标记软件，框选中书中的每一个黑色标记，尽量准确。保存标记文件，完成标记工作。
4. 新书图像和标记文件的批量生成，位置移动，只需点击其他准备工作按钮即可。
5. 开始训练，点击此按钮后开始学习新的图书。此过程大概需要一个小时左右。


## 知识补充

**QVariant转QString**

https://blog.csdn.net/soulwyb/article/details/100010074

    QVariant qv;
    QString qs = qv.toString();
 
**QString 转 QVariant**
 
    QString qs;
    QVariant qv(qs);


**1.QString转换String**

string s = qstr.toStdString();

**2.String转换QString**

QString qstr2 = QString::fromStdString(s);
**Qt使用多线程的一些心得——2.继承QObject的多线程使用方法**

https://blog.csdn.net/czyt1988/article/details/71194457
里面提到了弹窗报错`QueuedConnection`的问题
https://www.cnblogs.com/coolcpp/p/qt-thread.html
https://blog.csdn.net/u012372584/article/details/80509217


## 打包安装包
参考链接
https://www.cnblogs.com/dongh/p/6868638.html

查找依赖dll

找到vs安装目录下的dumpbin.exe 然后使用命令

    dumpbin /dependents QtMainWidget.exe

例如

    D:\Program Files (x86)\Microsoft Visual Studio 14.0>dumpbin /dependents D:\Code\QtMainWidget\x64\Release\QtMainWidget.exe

卸载的路径

    C：Windows\System32\Msiexec.exe
本例中缺少的依赖dll如下

*debug*

    opencv_world346d.dll
    MVSDKmd.dll
    ImageConvert.dll
    Qt5Widgetsd.dll
    Qt5Guid.dll
    Qt5Cored.dll
    SHELL32.dll
    KERNEL32.dll
    USER32.dll
    MSVCP140D.dll
    VCRUNTIME140D.dll
    ucrtbased.dll

*release*

    opencv_world346.dll
    MVSDKmd.dll
    ImageConvert.dll
    Qt5Widgets.dll
    Qt5Gui.dll
    Qt5Core.dll
    SHELL32.dll
    KERNEL32.dll
    USER32.dll
    MSVCP140.dll
    VCRUNTIME140.dll
    api-ms-win-crt-runtime-l1-1-0.dll
    api-ms-win-crt-heap-l1-1-0.dll
    api-ms-win-crt-stdio-l1-1-0.dll
    api-ms-win-crt-filesystem-l1-1-0.dll
    api-ms-win-crt-time-l1-1-0.dll
    api-ms-win-crt-math-l1-1-0.dll
    api-ms-win-crt-locale-l1-1-0.dll

额外的需求

    MSVCR120.dll
    GCBase_MD_VC120_v3_0.dll
    GenApi_MD_VC120_v3_0.dll
    CLProtocol_MD_VC120_v3_0.dll
    CONCRT140.dll
    MSVCP120.dll
    需要再加上yolo的dll

此外，还需要安装vc库VC_redist

生成的exe安装之后报错0xc000007b，不缺少依赖项
https://jingyan.baidu.com/article/375c8e1997252e25f2a22936.html
需要安装VC_redist.x64.exe vccredist.exe C++ 2013/C++2015的库

### 关于yolo_cpp_dll.dll
此dll是识别中的关键，依赖于[yolo](https://github.com/AlexeyAB/darknet#how-to-use-yolo-as-dll-and-so-libraries),在 `How to use Yolo as DLL and SO libraries`中写了如何得到dll，根据电脑显卡，自行生成dll文件放到exe同目录下



