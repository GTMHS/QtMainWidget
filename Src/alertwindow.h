#ifndef ALERTWINDOW_H
#define ALERTWINDOW_H

#include <QDialog>
#include <qtimer.h>

namespace Ui {
class AlertWindow;
}

class AlertWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AlertWindow(QWidget *parent = nullptr);
    ~AlertWindow();

	void startTimer();

private:
    Ui::AlertWindow *ui;
	QTimer *m_pTimer;
};

#endif // ALERTWINDOW_H
