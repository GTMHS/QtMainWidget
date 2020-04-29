#include "alertwindow.h"
#include "ui_alertwindow.h"

AlertWindow::AlertWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlertWindow)
{
    ui->setupUi(this);
	ui->label->setStyleSheet("color:red");

	m_pTimer = new QTimer(this);
	m_pTimer->setSingleShot(true);
	connect(m_pTimer, &QTimer::timeout, this, [=]() {this->close(); });
}

AlertWindow::~AlertWindow()
{
	if (m_pTimer != Q_NULLPTR)
		m_pTimer->deleteLater();
	delete ui;
}

void AlertWindow::startTimer() {
	m_pTimer->start(60000);
}

//²Î¿¼Á´½Óhttps://blog.csdn.net/xiezhongyuan07/article/details/79885546?utm_source=blogxgwz6