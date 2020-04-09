#include "parameterssetting.h"
#include "ui_parameterssetting.h"

ParametersSetting::ParametersSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParametersSetting)
{
    ui->setupUi(this);
	recevieData();
}

ParametersSetting::~ParametersSetting()
{
    delete ui;
}

void ParametersSetting::on_pushButton_clicked()
{
	double k = ui->lineEdit->text().toDouble();
	double b = ui->lineEdit_2->text().toDouble();
	double s = ui->lineEdit_3->text().toDouble();
	int n = ui->lineEdit_4->text().toInt();
	Config().Set("Line Fitting", "k", k);
	Config().Set("Line Fitting", "b", b);
	Config().Set("Line Fitting", "s", s);
	Config().Set("Line Fitting", "n", n);
	emit sendDataToMainWidget(k, b, s, n);
	this->close();
}

void ParametersSetting::recevieData()
{
	double k = Config().Get("Line Fitting", "k").toDouble();
	double b = Config().Get("Line Fitting", "b").toDouble();
	double s = Config().Get("Line Fitting", "s").toDouble();
	int n = Config().Get("Line Fitting", "n").toInt();
	ui->lineEdit->setText(QString::number(k));
	ui->lineEdit_2->setText(QString::number(b));
	ui->lineEdit_3->setText(QString::number(s));
	ui->lineEdit_4->setText(QString::number(n));
}
