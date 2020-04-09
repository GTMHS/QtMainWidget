#ifndef PARAMETERSSETTING_H
#define PARAMETERSSETTING_H

#include <QDialog>
#include <qsettings.h>
#include <Src\Config.h>

namespace Ui {
class ParametersSetting;
}

class ParametersSetting : public QDialog
{
    Q_OBJECT

public:
    explicit ParametersSetting(QWidget *parent = nullptr);
    ~ParametersSetting();

signals:
	void sendDataToMainWidget(double, double, double, int);

public slots:
	void recevieData();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ParametersSetting *ui;
};

#endif // PARAMETERSSETTING_H
