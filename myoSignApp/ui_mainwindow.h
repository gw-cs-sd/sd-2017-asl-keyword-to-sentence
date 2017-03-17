/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionsetgesturename;
    QWidget *centralWidget;
    QLabel *gestureNameOutput;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_2;
    QPlainTextEdit *outputData;
    QLabel *testsRemainOutput;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *gesturenameLabel;
    QLineEdit *gesturename;
    QLabel *testlabel;
    QPushButton *stop;
    QLabel *gestureLabel;
    QPushButton *start;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *testNumLabel;
    QLineEdit *testNum;
    QPushButton *startTraining;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(500, 540);
        actionsetgesturename = new QAction(MainWindow);
        actionsetgesturename->setObjectName(QStringLiteral("actionsetgesturename"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gestureNameOutput = new QLabel(centralWidget);
        gestureNameOutput->setObjectName(QStringLiteral("gestureNameOutput"));
        gestureNameOutput->setGeometry(QRect(80, 100, 101, 16));
        verticalLayoutWidget_2 = new QWidget(centralWidget);
        verticalLayoutWidget_2->setObjectName(QStringLiteral("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(10, 120, 481, 341));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        scrollArea_2 = new QScrollArea(verticalLayoutWidget_2);
        scrollArea_2->setObjectName(QStringLiteral("scrollArea_2"));
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QStringLiteral("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 477, 337));
        outputData = new QPlainTextEdit(scrollAreaWidgetContents_2);
        outputData->setObjectName(QStringLiteral("outputData"));
        outputData->setGeometry(QRect(0, 0, 481, 341));
        scrollArea_2->setWidget(scrollAreaWidgetContents_2);

        verticalLayout_2->addWidget(scrollArea_2);

        testsRemainOutput = new QLabel(centralWidget);
        testsRemainOutput->setObjectName(QStringLiteral("testsRemainOutput"));
        testsRemainOutput->setGeometry(QRect(290, 100, 59, 16));
        horizontalLayoutWidget = new QWidget(centralWidget);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 0, 241, 71));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        gesturenameLabel = new QLabel(horizontalLayoutWidget);
        gesturenameLabel->setObjectName(QStringLiteral("gesturenameLabel"));

        horizontalLayout->addWidget(gesturenameLabel);

        gesturename = new QLineEdit(horizontalLayoutWidget);
        gesturename->setObjectName(QStringLiteral("gesturename"));

        horizontalLayout->addWidget(gesturename);

        testlabel = new QLabel(centralWidget);
        testlabel->setObjectName(QStringLiteral("testlabel"));
        testlabel->setGeometry(QRect(190, 100, 101, 16));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        testlabel->setFont(font);
        stop = new QPushButton(centralWidget);
        stop->setObjectName(QStringLiteral("stop"));
        stop->setGeometry(QRect(380, 470, 113, 32));
        gestureLabel = new QLabel(centralWidget);
        gestureLabel->setObjectName(QStringLiteral("gestureLabel"));
        gestureLabel->setGeometry(QRect(10, 100, 59, 16));
        gestureLabel->setFont(font);
        start = new QPushButton(centralWidget);
        start->setObjectName(QStringLiteral("start"));
        start->setGeometry(QRect(270, 470, 113, 32));
        horizontalLayoutWidget_2 = new QWidget(centralWidget);
        horizontalLayoutWidget_2->setObjectName(QStringLiteral("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(260, 0, 231, 71));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        testNumLabel = new QLabel(horizontalLayoutWidget_2);
        testNumLabel->setObjectName(QStringLiteral("testNumLabel"));

        horizontalLayout_2->addWidget(testNumLabel);

        testNum = new QLineEdit(horizontalLayoutWidget_2);
        testNum->setObjectName(QStringLiteral("testNum"));

        horizontalLayout_2->addWidget(testNum);

        startTraining = new QPushButton(centralWidget);
        startTraining->setObjectName(QStringLiteral("startTraining"));
        startTraining->setGeometry(QRect(380, 70, 113, 32));
        startTraining->setFlat(false);
        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        startTraining->setDefault(false);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "myoSign", Q_NULLPTR));
        actionsetgesturename->setText(QApplication::translate("MainWindow", "setgesturename", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionsetgesturename->setToolTip(QApplication::translate("MainWindow", "Set the gesture name", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        gestureNameOutput->setText(QString());
        testsRemainOutput->setText(QString());
        gesturenameLabel->setText(QApplication::translate("MainWindow", "Gesture Name", Q_NULLPTR));
        gesturename->setText(QString());
        testlabel->setText(QApplication::translate("MainWindow", "Tests Remain:", Q_NULLPTR));
        stop->setText(QApplication::translate("MainWindow", "Stop", Q_NULLPTR));
        gestureLabel->setText(QApplication::translate("MainWindow", "Gesture:", Q_NULLPTR));
        start->setText(QApplication::translate("MainWindow", "Start", Q_NULLPTR));
        testNumLabel->setText(QApplication::translate("MainWindow", "Number of tests: ", Q_NULLPTR));
        startTraining->setText(QApplication::translate("MainWindow", "Start Training", Q_NULLPTR));
        mainToolBar->setWindowTitle(QApplication::translate("MainWindow", "myoSign", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
