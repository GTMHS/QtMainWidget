/********************************************************************************
** Form generated from reading UI file 'takephoto.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TAKEPHOTO_H
#define UI_TAKEPHOTO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_TakePhoto
{
public:
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton;

    void setupUi(QDialog *TakePhoto)
    {
        if (TakePhoto->objectName().isEmpty())
            TakePhoto->setObjectName(QString::fromUtf8("TakePhoto"));
        TakePhoto->resize(200, 50);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TakePhoto->sizePolicy().hasHeightForWidth());
        TakePhoto->setSizePolicy(sizePolicy);
        TakePhoto->setMinimumSize(QSize(200, 50));
        TakePhoto->setMaximumSize(QSize(200, 50));
        TakePhoto->setAutoFillBackground(false);
        TakePhoto->setSizeGripEnabled(false);
        TakePhoto->setModal(false);
        verticalLayout = new QVBoxLayout(TakePhoto);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        pushButton = new QPushButton(TakePhoto);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        verticalLayout->addWidget(pushButton);


        retranslateUi(TakePhoto);

        QMetaObject::connectSlotsByName(TakePhoto);
    } // setupUi

    void retranslateUi(QDialog *TakePhoto)
    {
        TakePhoto->setWindowTitle(QApplication::translate("TakePhoto", "\346\213\215\347\205\247", nullptr));
        pushButton->setText(QApplication::translate("TakePhoto", " \346\213\215\347\205\247", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TakePhoto: public Ui_TakePhoto {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TAKEPHOTO_H
