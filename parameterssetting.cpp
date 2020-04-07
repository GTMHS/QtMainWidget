#include "parameterssetting.h"
#include "ui_parameterssetting.h"

ParametersSetting::ParametersSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParametersSetting)
{
    ui->setupUi(this);
}

ParametersSetting::~ParametersSetting()
{
    delete ui;
}
