#pragma once
#include "qobject.h"
#include <Src\MySerialport.h>
#include <Src\alertwindow.h>


class MyThread : public QObject
{
	Q_OBJECT
public:
	MyThread(QObject *parent = 0);
	~MyThread();

	void MyWork();     //�̴߳���������
	void setFlag(bool flag = true);

signals:
	void signal_back();//�����������ź�


private:
	bool isStop;
	//������
	//MyCSerialPort mycserialport;

	AlertWindow *alertWindow;

};

//https://blog.csdn.net/weixin_38090427/article/details/83037020?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2.nonecase&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2.nonecase