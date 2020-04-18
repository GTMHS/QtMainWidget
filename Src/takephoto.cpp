#include "takephoto.h"
#include "ui_takephoto.h"

TakePhoto::TakePhoto(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TakePhoto)
{
    ui->setupUi(this);
}

TakePhoto::~TakePhoto()
{
    delete ui;
}

void TakePhoto::on_pushButton_clicked()
{
	emit sendTakePhoteToMainWidget();
	this->close();
}
