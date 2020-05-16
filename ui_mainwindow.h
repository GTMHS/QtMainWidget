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
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
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
    QVBoxLayout *verticalLayout_5;
    QSplitter *splitter;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_7;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_3;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout;
    QLabel *label_4;
    QLCDNumber *lcdNumber_2;
    QLabel *label_5;
    QLCDNumber *lcdNumber;
    QLabel *label_6;
    QLCDNumber *lcdNumber_3;
    QMenuBar *menuBar;
    QMenu *menutest;
    QMenu *menutest_2;
    QMenu *menu;
    QToolBar *toolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(923, 836);
        QIcon icon;
        icon.addFile(QString::fromUtf8("book_128px_1221557_easyicon.net.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        actiontest2 = new QAction(MainWindow);
        actiontest2->setObjectName(QString::fromUtf8("actiontest2"));
        actiontest3 = new QAction(MainWindow);
        actiontest3->setObjectName(QString::fromUtf8("actiontest3"));
        actionOPen_Camera = new QAction(MainWindow);
        actionOPen_Camera->setObjectName(QString::fromUtf8("actionOPen_Camera"));
        actionSavePic = new QAction(MainWindow);
        actionSavePic->setObjectName(QString::fromUtf8("actionSavePic"));
        actionSavePic->setEnabled(true);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("icon/camera.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSavePic->setIcon(icon1);
        actionLabel = new QAction(MainWindow);
        actionLabel->setObjectName(QString::fromUtf8("actionLabel"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8("icon/label.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLabel->setIcon(icon2);
        actionqita = new QAction(MainWindow);
        actionqita->setObjectName(QString::fromUtf8("actionqita"));
        actionTrain = new QAction(MainWindow);
        actionTrain->setObjectName(QString::fromUtf8("actionTrain"));
        actionTrain->setEnabled(false);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8("icon/train.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionTrain->setIcon(icon3);
        actionOpenCutWindow = new QAction(MainWindow);
        actionOpenCutWindow->setObjectName(QString::fromUtf8("actionOpenCutWindow"));
        actionCut_2 = new QAction(MainWindow);
        actionCut_2->setObjectName(QString::fromUtf8("actionCut_2"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8("icon/cut.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCut_2->setIcon(icon4);
        action = new QAction(MainWindow);
        action->setObjectName(QString::fromUtf8("action"));
        actioncans = new QAction(MainWindow);
        actioncans->setObjectName(QString::fromUtf8("actioncans"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8("icon/adjust.png"), QSize(), QIcon::Normal, QIcon::Off);
        actioncans->setIcon(icon5);
        actionPara = new QAction(MainWindow);
        actionPara->setObjectName(QString::fromUtf8("actionPara"));
        actionPara->setIcon(icon5);
        actionCut = new QAction(MainWindow);
        actionCut->setObjectName(QString::fromUtf8("actionCut"));
        actionCut->setIcon(icon4);
        actionDeleteOld = new QAction(MainWindow);
        actionDeleteOld->setObjectName(QString::fromUtf8("actionDeleteOld"));
        actionDeleteOld->setEnabled(false);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8("icon/delete.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionDeleteOld->setIcon(icon6);
        actionGetParemeter = new QAction(MainWindow);
        actionGetParemeter->setObjectName(QString::fromUtf8("actionGetParemeter"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8("icon/recognize.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionGetParemeter->setIcon(icon7);
        actiontemp = new QAction(MainWindow);
        actiontemp->setObjectName(QString::fromUtf8("actiontemp"));
        actiontakephoto = new QAction(MainWindow);
        actiontakephoto->setObjectName(QString::fromUtf8("actiontakephoto"));
        actiontakephoto->setIcon(icon1);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout_5 = new QVBoxLayout(centralWidget);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        splitter->setFont(font);
        splitter->setOrientation(Qt::Horizontal);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setFont(font);
        verticalLayout_2 = new QVBoxLayout(layoutWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(layoutWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setFont(font);
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setFont(font);
        label->setLineWidth(1);
        label->setMidLineWidth(0);
        label->setScaledContents(true);

        horizontalLayout->addWidget(label);


        verticalLayout_2->addWidget(groupBox);

        groupBox_2 = new QGroupBox(layoutWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setFont(font);
        verticalLayout = new QVBoxLayout(groupBox_2);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);
        label_2->setScaledContents(true);

        verticalLayout->addWidget(label_2);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font);
        label_3->setScaledContents(true);

        verticalLayout->addWidget(label_3);

        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setFont(font);

        verticalLayout->addWidget(label_7);

        verticalLayout->setStretch(0, 3);
        verticalLayout->setStretch(1, 1);
        verticalLayout->setStretch(2, 1);

        verticalLayout_2->addWidget(groupBox_2);

        verticalLayout_2->setStretch(0, 5);
        verticalLayout_2->setStretch(1, 2);
        splitter->addWidget(layoutWidget);
        layoutWidget1 = new QWidget(splitter);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setFont(font);
        verticalLayout_4 = new QVBoxLayout(layoutWidget1);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        groupBox_4 = new QGroupBox(layoutWidget1);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setFont(font);
        verticalLayout_3 = new QVBoxLayout(groupBox_4);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        pushButton = new QPushButton(groupBox_4);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMinimumSize(QSize(150, 0));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Adobe Devanagari"));
        font1.setPointSize(12);
        font1.setBold(true);
        font1.setWeight(75);
        pushButton->setFont(font1);

        verticalLayout_3->addWidget(pushButton);

        pushButton_2 = new QPushButton(groupBox_4);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setMinimumSize(QSize(150, 0));
        pushButton_2->setFont(font1);

        verticalLayout_3->addWidget(pushButton_2);

        pushButton_3 = new QPushButton(groupBox_4);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setMinimumSize(QSize(150, 0));
        pushButton_3->setFont(font1);

        verticalLayout_3->addWidget(pushButton_3);

        pushButton_4 = new QPushButton(groupBox_4);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setFont(font1);

        verticalLayout_3->addWidget(pushButton_4);


        verticalLayout_4->addWidget(groupBox_4);

        groupBox_3 = new QGroupBox(layoutWidget1);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setFont(font);
        gridLayout = new QGridLayout(groupBox_3);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Adobe Devanagari"));
        font2.setPointSize(15);
        font2.setBold(true);
        font2.setWeight(75);
        label_4->setFont(font2);

        gridLayout->addWidget(label_4, 0, 0, 1, 1);

        lcdNumber_2 = new QLCDNumber(groupBox_3);
        lcdNumber_2->setObjectName(QString::fromUtf8("lcdNumber_2"));
        lcdNumber_2->setFont(font1);

        gridLayout->addWidget(lcdNumber_2, 0, 1, 1, 1);

        label_5 = new QLabel(groupBox_3);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font2);

        gridLayout->addWidget(label_5, 1, 0, 1, 1);

        lcdNumber = new QLCDNumber(groupBox_3);
        lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));
        lcdNumber->setFont(font1);

        gridLayout->addWidget(lcdNumber, 1, 1, 1, 1);

        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setFont(font2);

        gridLayout->addWidget(label_6, 2, 0, 1, 1);

        lcdNumber_3 = new QLCDNumber(groupBox_3);
        lcdNumber_3->setObjectName(QString::fromUtf8("lcdNumber_3"));
        lcdNumber_3->setFont(font1);

        gridLayout->addWidget(lcdNumber_3, 2, 1, 1, 1);


        verticalLayout_4->addWidget(groupBox_3);

        splitter->addWidget(layoutWidget1);

        verticalLayout_5->addWidget(splitter);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 923, 26));
        menutest = new QMenu(menuBar);
        menutest->setObjectName(QString::fromUtf8("menutest"));
        menutest_2 = new QMenu(menuBar);
        menutest_2->setObjectName(QString::fromUtf8("menutest_2"));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu->setEnabled(true);
        MainWindow->setMenuBar(menuBar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);
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
        toolBar->addAction(actionSavePic);
        toolBar->addAction(actionCut_2);
        toolBar->addAction(actionGetParemeter);
        toolBar->addAction(actionPara);
        toolBar->addSeparator();
        toolBar->addAction(actionLabel);
        toolBar->addAction(actionTrain);
        toolBar->addAction(actionDeleteOld);
        toolBar->addSeparator();

        retranslateUi(MainWindow);
        QObject::connect(actionOPen_Camera, SIGNAL(triggered()), MainWindow, SLOT(showMaximized()));
        QObject::connect(action, SIGNAL(triggered()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\345\233\276\344\271\246\346\243\200\346\265\213\347\263\273\347\273\237", nullptr));
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
        groupBox->setTitle(QApplication::translate("MainWindow", "\345\256\236\346\227\266\347\224\273\351\235\242", nullptr));
        label->setText(QString());
        groupBox_2->setTitle(QApplication::translate("MainWindow", "\345\233\276\344\271\246\351\242\204\346\265\213", nullptr));
        label_2->setText(QString());
        label_3->setText(QString());
        label_7->setText(QString());
        groupBox_4->setTitle(QApplication::translate("MainWindow", "\345\270\270\347\224\250\346\223\215\344\275\234\346\216\247\345\210\266\345\214\272", nullptr));
        pushButton->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\347\233\270\346\234\272", nullptr));
        pushButton_2->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255\347\233\270\346\234\272", nullptr));
        pushButton_3->setText(QApplication::translate("MainWindow", "\345\274\200\345\247\213\350\257\206\345\210\253", nullptr));
        pushButton_4->setText(QApplication::translate("MainWindow", "\345\201\234\346\255\242\350\257\206\345\210\253", nullptr));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "\346\225\260\351\207\217\346\230\276\347\244\272", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "\346\200\273\346\225\260", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "\346\255\243\347\241\256\346\200\273\346\225\260", nullptr));
        label_6->setText(QApplication::translate("MainWindow", "\351\224\231\350\257\257\346\200\273\346\225\260", nullptr));
        menutest->setTitle(QApplication::translate("MainWindow", "\346\226\207\344\273\266", nullptr));
        menutest_2->setTitle(QApplication::translate("MainWindow", "\346\227\245\345\270\270\344\275\277\347\224\250", nullptr));
        menu->setTitle(QApplication::translate("MainWindow", "\346\226\260\344\271\246\350\256\255\347\273\203", nullptr));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
