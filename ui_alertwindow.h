/********************************************************************************
** Form generated from reading UI file 'alertwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ALERTWINDOW_H
#define UI_ALERTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AlertWindow
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *AlertWindow)
    {
        if (AlertWindow->objectName().isEmpty())
            AlertWindow->setObjectName(QString::fromUtf8("AlertWindow"));
        AlertWindow->resize(592, 280);
        verticalLayout = new QVBoxLayout(AlertWindow);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(AlertWindow);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(61);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        buttonBox = new QDialogButtonBox(AlertWindow);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(AlertWindow);
        QObject::connect(buttonBox, SIGNAL(accepted()), AlertWindow, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), AlertWindow, SLOT(reject()));

        QMetaObject::connectSlotsByName(AlertWindow);
    } // setupUi

    void retranslateUi(QDialog *AlertWindow)
    {
        AlertWindow->setWindowTitle(QApplication::translate("AlertWindow", "\345\233\276\344\271\246\350\243\205\350\256\242\346\234\211\350\257\257", nullptr));
        label->setText(QApplication::translate("AlertWindow", "\345\233\276\344\271\246\350\243\205\350\256\242\346\234\211\350\257\257\357\274\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AlertWindow: public Ui_AlertWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ALERTWINDOW_H
