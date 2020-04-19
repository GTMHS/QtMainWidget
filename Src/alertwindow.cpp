#include "alertwindow.h"
#include "ui_alertwindow.h"

AlertWindow::AlertWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlertWindow)
{
    ui->setupUi(this);
	ui->label->setStyleSheet("color:red");
}

AlertWindow::~AlertWindow()
{
    delete ui;
}
