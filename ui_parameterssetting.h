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
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_ParametersSetting
{
public:
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
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
    QLabel *label_6;
    QLineEdit *lineEdit_6;
    QLabel *label_7;
    QLineEdit *lineEdit_7;
    QPushButton *pushButton;

    void setupUi(QDialog *ParametersSetting)
    {
        if (ParametersSetting->objectName().isEmpty())
            ParametersSetting->setObjectName(QString::fromUtf8("ParametersSetting"));
        ParametersSetting->resize(236, 306);
        QIcon icon;
        icon.addFile(QString::fromUtf8("icon/adjust.png"), QSize(), QIcon::Normal, QIcon::Off);
        ParametersSetting->setWindowIcon(icon);
        gridLayout = new QGridLayout(ParametersSetting);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox = new QGroupBox(ParametersSetting);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        radioButton = new QRadioButton(groupBox);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));

        horizontalLayout->addWidget(radioButton);

        radioButton_2 = new QRadioButton(groupBox);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));

        horizontalLayout->addWidget(radioButton_2);


        gridLayout->addWidget(groupBox, 0, 0, 1, 2);

        label = new QLabel(ParametersSetting);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 1, 0, 1, 1);

        lineEdit = new QLineEdit(ParametersSetting);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout->addWidget(lineEdit, 1, 1, 2, 1);

        label_2 = new QLabel(ParametersSetting);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 3, 0, 1, 1);

        lineEdit_2 = new QLineEdit(ParametersSetting);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        gridLayout->addWidget(lineEdit_2, 3, 1, 1, 1);

        label_3 = new QLabel(ParametersSetting);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 4, 0, 1, 1);

        lineEdit_3 = new QLineEdit(ParametersSetting);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));

        gridLayout->addWidget(lineEdit_3, 4, 1, 1, 1);

        label_4 = new QLabel(ParametersSetting);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 5, 0, 1, 1);

        lineEdit_4 = new QLineEdit(ParametersSetting);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));

        gridLayout->addWidget(lineEdit_4, 5, 1, 1, 1);

        label_5 = new QLabel(ParametersSetting);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 6, 0, 1, 1);

        lineEdit_5 = new QLineEdit(ParametersSetting);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));

        gridLayout->addWidget(lineEdit_5, 6, 1, 1, 1);

        label_6 = new QLabel(ParametersSetting);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 7, 0, 1, 1);

        lineEdit_6 = new QLineEdit(ParametersSetting);
        lineEdit_6->setObjectName(QString::fromUtf8("lineEdit_6"));

        gridLayout->addWidget(lineEdit_6, 7, 1, 1, 1);

        label_7 = new QLabel(ParametersSetting);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 8, 0, 1, 1);

        lineEdit_7 = new QLineEdit(ParametersSetting);
        lineEdit_7->setObjectName(QString::fromUtf8("lineEdit_7"));

        gridLayout->addWidget(lineEdit_7, 8, 1, 1, 1);

        pushButton = new QPushButton(ParametersSetting);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout->addWidget(pushButton, 9, 0, 1, 2);


        retranslateUi(ParametersSetting);

        QMetaObject::connectSlotsByName(ParametersSetting);
    } // setupUi

    void retranslateUi(QDialog *ParametersSetting)
    {
        ParametersSetting->setWindowTitle(QApplication::translate("ParametersSetting", "\345\217\202\346\225\260\350\256\276\347\275\256", nullptr));
        groupBox->setTitle(QApplication::translate("ParametersSetting", "\351\273\221\350\211\262\346\240\207\350\256\260\345\275\242\347\212\266", nullptr));
        radioButton->setText(QApplication::translate("ParametersSetting", "\347\233\264\347\272\277\345\236\213", nullptr));
        radioButton_2->setText(QApplication::translate("ParametersSetting", "V\345\255\227\345\275\242", nullptr));
        label->setText(QApplication::translate("ParametersSetting", "\345\217\202\346\225\2601\357\274\210\346\226\234\347\216\207k\357\274\211", nullptr));
        label_2->setText(QApplication::translate("ParametersSetting", "\345\217\202\346\225\2602\357\274\210\346\210\252\350\267\235b\357\274\211", nullptr));
        label_3->setText(QApplication::translate("ParametersSetting", "\345\217\202\346\225\2603\357\274\210\347\263\273\346\225\260s\357\274\211", nullptr));
        label_4->setText(QApplication::translate("ParametersSetting", "\351\273\221\345\235\227\346\225\260\351\207\217\357\274\210n\357\274\211", nullptr));
        label_5->setText(QApplication::translate("ParametersSetting", "\345\217\202\346\225\2601\351\230\210\345\200\274", nullptr));
        label_6->setText(QApplication::translate("ParametersSetting", "\345\217\202\346\225\2602\351\230\210\345\200\274", nullptr));
        label_7->setText(QApplication::translate("ParametersSetting", "\345\217\202\346\225\2603\351\230\210\345\200\274", nullptr));
        pushButton->setText(QApplication::translate("ParametersSetting", "\347\241\256\345\256\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ParametersSetting: public Ui_ParametersSetting {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PARAMETERSSETTING_H
