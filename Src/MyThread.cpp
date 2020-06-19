#include "MyThread.h"
#include <QThread>
#include <QDebug>
#include <qmessagebox.h>
//bool revFlag = false;

MyThread::MyThread(QObject *parent) : QObject(parent)
{
	isStop = false;
	//串口类
	//外部设备初始化及监听
	//mycserialport.InitPort();
	//mycserialport.OpenListenThread();
}

MyThread::~MyThread()
{
}

void MyThread::MyWork()        //线程处理函数：具体处理的事情
{
	//while (!isStop)
	//{
	//	QThread::sleep(1);
	//	emit signal_back();    //发送返回信号
	//	qDebug() << "the child thread number:" << QThread::currentThread();
	//}
	//bool revFlag = false;
	//unsigned char uc[] = { 0x7e,0x01,0x55,0x55,0x0d,0x0d };
	//int count = 0;
	//while (revFlag != true) {
	//	revFlag = mycserialport.WriteData(uc, 6);
	//	Sleep(50);
	//	count++;
	//	if (count >= 3) {
	//		//cout << "未收到下位机确认信息!" << endl;
	//		//连续发三次，三次握手,返回动作执行成功
	//		count = 0;
	//		break;
	//	}
	//}
	//revFlag = false;
	//Sleep(2000);

	alertWindow = new AlertWindow;
	alertWindow->startTimer();
	alertWindow->show();

	emit signal_back();    //发送返回信号
}

void MyThread::setFlag(bool flag)
{
	isStop = flag;
}
