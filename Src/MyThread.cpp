#include "MyThread.h"
#include <QThread>
#include <QDebug>
#include <qmessagebox.h>
//bool revFlag = false;

MyThread::MyThread(QObject *parent) : QObject(parent)
{
	isStop = false;
	//������
	//�ⲿ�豸��ʼ��������
	//mycserialport.InitPort();
	//mycserialport.OpenListenThread();
}

MyThread::~MyThread()
{
}

void MyThread::MyWork()        //�̴߳����������崦�������
{
	//while (!isStop)
	//{
	//	QThread::sleep(1);
	//	emit signal_back();    //���ͷ����ź�
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
	//		//cout << "δ�յ���λ��ȷ����Ϣ!" << endl;
	//		//���������Σ���������,���ض���ִ�гɹ�
	//		count = 0;
	//		break;
	//	}
	//}
	//revFlag = false;
	//Sleep(2000);

	alertWindow = new AlertWindow;
	alertWindow->startTimer();
	alertWindow->show();

	emit signal_back();    //���ͷ����ź�
}

void MyThread::setFlag(bool flag)
{
	isStop = flag;
}
