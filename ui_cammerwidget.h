/********************************************************************************
** Form generated from reading UI file 'cammerwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMMERWIDGET_H
#define UI_CAMMERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CammerWidget
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QFrame *line;
    QLabel *label_2;
    QLabel *label_3;

    void setupUi(QWidget *CammerWidget)
    {
        if (CammerWidget->objectName().isEmpty())
            CammerWidget->setObjectName(QStringLiteral("CammerWidget"));
        CammerWidget->resize(618, 663);
        QFont font;
        font.setPointSize(18);
        font.setBold(true);
        font.setWeight(75);
        CammerWidget->setFont(font);
        verticalLayout = new QVBoxLayout(CammerWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(CammerWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(600, 400));
        label->setMaximumSize(QSize(2048, 1920));
        label->setScaledContents(true);

        verticalLayout->addWidget(label);

        line = new QFrame(CammerWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setEnabled(true);
        line->setLayoutDirection(Qt::LeftToRight);
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        label_2 = new QLabel(CammerWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMinimumSize(QSize(600, 200));
        label_2->setMaximumSize(QSize(2048, 1920));
        label_2->setScaledContents(true);

        verticalLayout->addWidget(label_2);

        label_3 = new QLabel(CammerWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout->addWidget(label_3);

        verticalLayout->setStretch(0, 4);
        verticalLayout->setStretch(1, 2);
        verticalLayout->setStretch(2, 1);

        retranslateUi(CammerWidget);

        QMetaObject::connectSlotsByName(CammerWidget);
    } // setupUi

    void retranslateUi(QWidget *CammerWidget)
    {
        CammerWidget->setWindowTitle(QApplication::translate("CammerWidget", "CammerWidget", Q_NULLPTR));
        label->setText(QString());
        label_2->setText(QString());
        label_3->setText(QApplication::translate("CammerWidget", "TextLabel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CammerWidget: public Ui_CammerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMMERWIDGET_H
