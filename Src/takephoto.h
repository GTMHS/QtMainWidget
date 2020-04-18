#ifndef TAKEPHOTO_H
#define TAKEPHOTO_H

#include <QDialog>

namespace Ui {
class TakePhoto;
}

class TakePhoto : public QDialog
{
    Q_OBJECT

public:
    explicit TakePhoto(QWidget *parent = nullptr);
    ~TakePhoto();

signals:
	void sendTakePhoteToMainWidget();
private slots:
    void on_pushButton_clicked();

private:
    Ui::TakePhoto *ui;
};

#endif // TAKEPHOTO_H
