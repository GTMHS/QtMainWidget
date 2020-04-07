#ifndef PARAMETERSSETTING_H
#define PARAMETERSSETTING_H

#include <QDialog>

namespace Ui {
class ParametersSetting;
}

class ParametersSetting : public QDialog
{
    Q_OBJECT

public:
    explicit ParametersSetting(QWidget *parent = nullptr);
    ~ParametersSetting();

private:
    Ui::ParametersSetting *ui;
};

#endif // PARAMETERSSETTING_H
