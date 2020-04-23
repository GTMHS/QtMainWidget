/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actiontest2;
    QAction *actiontest3;
    QAction *actionOPen_Camera;
    QAction *actionSavePic;
    QAction *actionLabel;
    QAction *actionqita;
    QAction *actionTrain;
    QAction *actionOpenCutWindow;
    QAction *actionCut_2;
    QAction *action;
    QAction *actioncans;
    QAction *actionPara;
    QAction *actionCut;
    QAction *actionDeleteOld;
    QAction *actionGetParemeter;
    QAction *actiontemp;
    QAction *actiontakephoto;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QWidget *layoutWidget1;
    QFormLayout *formLayout;
    QLabel *label_4;
    QLCDNumber *lcdNumber_2;
    QLCDNumber *lcdNumber;
    QLCDNumber *lcdNumber_3;
    QSpacerItem *verticalSpacer;
    QLabel *label_5;
    QLabel *label_6;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QMenuBar *menuBar;
    QMenu *menutest;
    QMenu *menutest_2;
    QMenu *menu;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(775, 739);
        actiontest2 = new QAction(MainWindow);
        actiontest2->setObjectName(QString::fromUtf8("actiontest2"));
        actiontest3 = new QAction(MainWindow);
        actiontest3->setObjectName(QString::fromUtf8("actiontest3"));
        actionOPen_Camera = new QAction(MainWindow);
        actionOPen_Camera->setObjectName(QString::fromUtf8("actionOPen_Camera"));
        actionSavePic = new QAction(MainWindow);
        actionSavePic->setObjectName(QString::fromUtf8("actionSavePic"));
        actionSavePic->setEnabled(true);
        actionLabel = new QAction(MainWindow);
        actionLabel->setObjectName(QString::fromUtf8("actionLabel"));
        actionqita = new QAction(MainWindow);
        actionqita->setObjectName(QString::fromUtf8("actionqita"));
        actionTrain = new QAction(MainWindow);
        actionTrain->setObjectName(QString::fromUtf8("actionTrain"));
        actionTrain->setEnabled(false);
        actionOpenCutWindow = new QAction(MainWindow);
        actionOpenCutWindow->setObjectName(QString::fromUtf8("actionOpenCutWindow"));
        actionCut_2 = new QAction(MainWindow);
        actionCut_2->setObjectName(QString::fromUtf8("actionCut_2"));
        action = new QAction(MainWindow);
        action->setObjectName(QString::fromUtf8("action"));
        actioncans = new QAction(MainWindow);
        actioncans->setObjectName(QString::fromUtf8("actioncans"));
        actionPara = new QAction(MainWindow);
        actionPara->setObjectName(QString::fromUtf8("actionPara"));
        actionCut = new QAction(MainWindow);
        actionCut->setObjectName(QString::fromUtf8("actionCut"));
        actionDeleteOld = new QAction(MainWindow);
        actionDeleteOld->setObjectName(QString::fromUtf8("actionDeleteOld"));
        actionDeleteOld->setEnabled(false);
        actionGetParemeter = new QAction(MainWindow);
        actionGetParemeter->setObjectName(QString::fromUtf8("actionGetParemeter"));
        actiontemp = new QAction(MainWindow);
        actiontemp->setObjectName(QString::fromUtf8("actiontemp"));
        actiontakephoto = new QAction(MainWindow);
        actiontakephoto->setObjectName(QString::fromUtf8("actiontakephoto"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        splitter->setOpaqueResize(false);
        splitter->setChildrenCollapsible(true);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        verticalLayout_2 = new QVBoxLayout(layoutWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setLineWidth(1);
        label->setMidLineWidth(0);
        label->setScaledContents(true);

        verticalLayout_2->addWidget(label);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setScaledContents(true);

        verticalLayout_2->addWidget(label_2);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QFont font;
        font.setPointSize(15);
        label_3->setFont(font);
        label_3->setScaledContents(true);

        verticalLayout_2->addWidget(label_3);

        verticalLayout_2->setStretch(0, 3);
        verticalLayout_2->setStretch(1, 1);
        verticalLayout_2->setStretch(2, 1);
        splitter->addWidget(layoutWidget);
        layoutWidget1 = new QWidget(splitter);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        formLayout = new QFormLayout(layoutWidget1);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(layoutWidget1);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Adobe Devanagari"));
        font1.setPointSize(14);
        label_4->setFont(font1);

        formLayout->setWidget(5, QFormLayout::LabelRole, label_4);

        lcdNumber_2 = new QLCDNumber(layoutWidget1);
        lcdNumber_2->setObjectName(QString::fromUtf8("lcdNumber_2"));
        lcdNumber_2->setFont(font1);

        formLayout->setWidget(5, QFormLayout::FieldRole, lcdNumber_2);

        lcdNumber = new QLCDNumber(layoutWidget1);
        lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));
        lcdNumber->setFont(font1);

        formLayout->setWidget(6, QFormLayout::FieldRole, lcdNumber);

        lcdNumber_3 = new QLCDNumber(layoutWidget1);
        lcdNumber_3->setObjectName(QString::fromUtf8("lcdNumber_3"));
        lcdNumber_3->setFont(font1);

        formLayout->setWidget(7, QFormLayout::FieldRole, lcdNumber_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        formLayout->setItem(8, QFormLayout::FieldRole, verticalSpacer);

        label_5 = new QLabel(layoutWidget1);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font1);

        formLayout->setWidget(6, QFormLayout::LabelRole, label_5);

        label_6 = new QLabel(layoutWidget1);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setFont(font1);

        formLayout->setWidget(7, QFormLayout::LabelRole, label_6);

        pushButton = new QPushButton(layoutWidget1);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMinimumSize(QSize(150, 0));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Adobe Devanagari"));
        font2.setPointSize(11);
        pushButton->setFont(font2);

        formLayout->setWidget(1, QFormLayout::SpanningRole, pushButton);

        pushButton_2 = new QPushButton(layoutWidget1);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setMinimumSize(QSize(150, 0));
        pushButton_2->setFont(font2);

        formLayout->setWidget(2, QFormLayout::SpanningRole, pushButton_2);

        pushButton_3 = new QPushButton(layoutWidget1);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setMinimumSize(QSize(150, 0));
        pushButton_3->setFont(font2);

        formLayout->setWidget(3, QFormLayout::SpanningRole, pushButton_3);

        pushButton_4 = new QPushButton(layoutWidget1);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setFont(font2);

        formLayout->setWidget(4, QFormLayout::SpanningRole, pushButton_4);

        splitter->addWidget(layoutWidget1);

        horizontalLayout->addWidget(splitter);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 775, 23));
        menutest = new QMenu(menuBar);
        menutest->setObjectName(QString::fromUtf8("menutest"));
        menutest_2 = new QMenu(menuBar);
        menutest_2->setObjectName(QString::fromUtf8("menutest_2"));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu->setEnabled(true);
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menutest->menuAction());
        menuBar->addAction(menutest_2->menuAction());
        menuBar->addAction(menu->menuAction());
        menutest->addAction(actionOPen_Camera);
        menutest->addAction(actioncans);
        menutest->addSeparator();
        menutest->addAction(action);
        menutest_2->addAction(actiontakephoto);
        menutest_2->addAction(actionCut);
        menutest_2->addAction(actionGetParemeter);
        menutest_2->addAction(actionPara);
        menu->addAction(actionSavePic);
        menu->addAction(actionCut_2);
        menu->addAction(actionLabel);
        menu->addAction(actionTrain);
        menu->addAction(actionDeleteOld);

        retranslateUi(MainWindow);
        QObject::connect(actionOPen_Camera, SIGNAL(triggered()), MainWindow, SLOT(showMaximized()));
        QObject::connect(action, SIGNAL(triggered()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        actiontest2->setText(QApplication::translate("MainWindow", "\346\226\207\344\273\266", nullptr));
        actiontest3->setText(QApplication::translate("MainWindow", "\351\242\204\347\225\231", nullptr));
        actionOPen_Camera->setText(QApplication::translate("MainWindow", "\345\205\250\345\261\217", nullptr));
        actionSavePic->setText(QApplication::translate("MainWindow", "\346\226\260\344\271\246\346\213\215\347\205\247", nullptr));
        actionLabel->setText(QApplication::translate("MainWindow", "\346\226\260\344\271\246\346\240\207\350\256\260", nullptr));
        actionqita->setText(QApplication::translate("MainWindow", "\345\205\266\344\273\226\345\207\206\345\244\207\345\267\245\344\275\234", nullptr));
        actionTrain->setText(QApplication::translate("MainWindow", "\345\274\200\345\247\213\350\256\255\347\273\203", nullptr));
        actionOpenCutWindow->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\350\243\201\345\211\252\347\252\227\345\217\243", nullptr));
        actionCut_2->setText(QApplication::translate("MainWindow", "\350\243\201\345\211\252\345\233\276\347\211\207", nullptr));
        action->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255", nullptr));
        actioncans->setText(QApplication::translate("MainWindow", "\345\217\202\346\225\260\350\260\203\346\225\264", nullptr));
        actionPara->setText(QApplication::translate("MainWindow", "\345\217\202\346\225\260\350\260\203\346\225\264", nullptr));
        actionCut->setText(QApplication::translate("MainWindow", "\350\243\201\345\211\252\345\233\276\347\211\207", nullptr));
        actionDeleteOld->setText(QApplication::translate("MainWindow", "\345\210\240\351\231\244\350\256\255\347\273\203\346\240\267\346\234\254", nullptr));
        actionGetParemeter->setText(QApplication::translate("MainWindow", "\350\257\206\345\210\253\345\217\202\346\225\260", nullptr));
        actiontemp->setText(QApplication::translate("MainWindow", "temp", nullptr));
        actiontakephoto->setText(QApplication::translate("MainWindow", "\346\226\260\344\271\246\346\213\215\347\205\247", nullptr));
        label->setText(QApplication::translate("MainWindow", "TextLabel1", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "TextLabel2", nullptr));
        label_3->setText(QString());
        label_4->setText(QApplication::translate("MainWindow", "\346\200\273\346\225\260", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "\346\255\243\347\241\256\346\200\273\346\225\260", nullptr));
        label_6->setText(QApplication::translate("MainWindow", "\351\224\231\350\257\257\346\200\273\346\225\260", nullptr));
        pushButton->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\347\233\270\346\234\272", nullptr));
        pushButton_2->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255\347\233\270\346\234\272", nullptr));
        pushButton_3->setText(QApplication::translate("MainWindow", "\345\274\200\345\247\213\350\257\206\345\210\253", nullptr));
        pushButton_4->setText(QApplication::translate("MainWindow", "\345\201\234\346\255\242\350\257\206\345\210\253", nullptr));
        menutest->setTitle(QApplication::translate("MainWindow", "\346\226\207\344\273\266", nullptr));
        menutest_2->setTitle(QApplication::translate("MainWindow", "\346\227\245\345\270\270\344\275\277\347\224\250", nullptr));
        menu->setTitle(QApplication::translate("MainWindow", "\346\226\260\344\271\246\350\256\255\347\273\203", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
