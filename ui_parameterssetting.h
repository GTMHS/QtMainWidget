/********************************************************************************
** Form generated from reading UI file 'parameterssetting.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PARAMETERSSETTING_H
#define UI_PARAMETERSSETTING_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ParametersSetting
{
public:
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QLabel *label_3;
    QLineEdit *lineEdit_3;
    QLabel *label_4;
    QLineEdit *lineEdit_4;
    QLabel *label_5;
    QLineEdit *lineEdit_5;
    QPushButton *pushButton;
    QLabel *label_6;
    QLabel *label_7;
    QLineEdit *lineEdit_6;
    QLineEdit *lineEdit_7;

    void setupUi(QDialog *ParametersSetting)
    {
        if (ParametersSetting->objectName().isEmpty())
            ParametersSetting->setObjectName(QString::fromUtf8("ParametersSetting"));
        ParametersSetting->resize(211, 223);
        formLayout = new QFormLayout(ParametersSetting);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label = new QLabel(ParametersSetting);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        lineEdit = new QLineEdit(ParametersSetting);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEdit);

        label_2 = new QLabel(ParametersSetting);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        lineEdit_2 = new QLineEdit(ParametersSetting);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        formLayout->setWidget(1, QFormLayout::FieldRole, lineEdit_2);

        label_3 = new QLabel(ParametersSetting);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        lineEdit_3 = new QLineEdit(ParametersSetting);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));

        formLayout->setWidget(2, QFormLayout::FieldRole, lineEdit_3);

        label_4 = new QLabel(ParametersSetting);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_4);

        lineEdit_4 = new QLineEdit(ParametersSetting);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));

        formLayout->setWidget(3, QFormLayout::FieldRole, lineEdit_4);

        label_5 = new QLabel(ParametersSetting);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label_5);

        lineEdit_5 = new QLineEdit(ParametersSetting);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));

        formLayout->setWidget(4, QFormLayout::FieldRole, lineEdit_5);

        pushButton = new QPushButton(ParametersSetting);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        formLayout->setWidget(7, QFormLayout::SpanningRole, pushButton);

        label_6 = new QLabel(ParametersSetting);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        formLayout->setWidget(5, QFormLayout::LabelRole, label_6);

        label_7 = new QLabel(ParametersSetting);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        formLayout->setWidget(6, QFormLayout::LabelRole, label_7);

        lineEdit_6 = new QLineEdit(ParametersSetting);
        lineEdit_6->setObjectName(QString::fromUtf8("lineEdit_6"));

        formLayout->setWidget(5, QFormLayout::FieldRole, lineEdit_6);

        lineEdit_7 = new QLineEdit(ParametersSetting);
        lineEdit_7->setObjectName(QString::fromUtf8("lineEdit_7"));

        formLayout->setWidget(6, QFormLayout::FieldRole, lineEdit_7);


        retranslateUi(ParametersSetting);

        QMetaObject::connectSlotsByName(ParametersSetting);
    } // setupUi

    void retranslateUi(QDialog *ParametersSetting)
    {
        ParametersSetting->setWindowTitle(QApplication::translate("ParametersSetting", "\345\217\202\346\225\260\350\256\276\347\275\256", nullptr));
        label->setText(QApplication::translate("ParametersSetting", "\345\217\202\346\225\2601\357\274\210\346\226\234\347\216\207k\357\274\211", nullptr));
        label_2->setText(QApplication::translate("ParametersSetting", "\345\217\202\346\225\2602\357\274\210\346\210\252\350\267\235b\357\274\211", nullptr));
        label_3->setText(QApplication::translate("ParametersSetting", "\345\217\202\346\225\2603\357\274\210\347\263\273\346\225\260s\357\274\211", nullptr));
        label_4->setText(QApplication::translate("ParametersSetting", "\351\273\221\345\235\227\346\225\260\351\207\217\357\274\210n\357\274\211", nullptr));
        label_5->setText(QApplication::translate("ParametersSetting", "\345\217\202\346\225\2601\351\230\210\345\200\274", nullptr));
        pushButton->setText(QApplication::translate("ParametersSetting", "\347\241\256\345\256\232", nullptr));
        label_6->setText(QApplication::translate("ParametersSetting", "\345\217\202\346\225\2602\351\230\210\345\200\274", nullptr));
        label_7->setText(QApplication::translate("ParametersSetting", "\345\217\202\346\225\2603\351\230\210\345\200\274", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ParametersSetting: public Ui_ParametersSetting {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PARAMETERSSETTING_H
