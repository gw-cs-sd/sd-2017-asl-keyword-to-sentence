/********************************************************************************
** Form generated from reading UI file 'displaywords.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DISPLAYWORDS_H
#define UI_DISPLAYWORDS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_displayWords
{
public:
    QWidget *centralWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLabel *displayTheWord;
    QPushButton *startButton;
    QMenuBar *menuBar;
    QMenu *menumyoSign;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *displayWords)
    {
        if (displayWords->objectName().isEmpty())
            displayWords->setObjectName(QStringLiteral("displayWords"));
        displayWords->resize(400, 363);
        centralWidget = new QWidget(displayWords);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 60, 381, 221));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setMaximumSize(QSize(16777215, 20));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setAutoFillBackground(true);

        verticalLayout->addWidget(label);

        displayTheWord = new QLabel(verticalLayoutWidget);
        displayTheWord->setObjectName(QStringLiteral("displayTheWord"));
        QFont font1;
        font1.setPointSize(30);
        displayTheWord->setFont(font1);
        displayTheWord->setAutoFillBackground(true);
        displayTheWord->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(displayTheWord);

        startButton = new QPushButton(centralWidget);
        startButton->setObjectName(QStringLiteral("startButton"));
        startButton->setGeometry(QRect(140, 10, 113, 32));
        displayWords->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(displayWords);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 22));
        menumyoSign = new QMenu(menuBar);
        menumyoSign->setObjectName(QStringLiteral("menumyoSign"));
        displayWords->setMenuBar(menuBar);
        mainToolBar = new QToolBar(displayWords);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        displayWords->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(displayWords);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        displayWords->setStatusBar(statusBar);

        menuBar->addAction(menumyoSign->menuAction());

        retranslateUi(displayWords);

        QMetaObject::connectSlotsByName(displayWords);
    } // setupUi

    void retranslateUi(QMainWindow *displayWords)
    {
        displayWords->setWindowTitle(QApplication::translate("displayWords", "displayWords", Q_NULLPTR));
        label->setText(QApplication::translate("displayWords", "Performed gesture:", Q_NULLPTR));
        displayTheWord->setText(QString());
        startButton->setText(QApplication::translate("displayWords", "Start", Q_NULLPTR));
        menumyoSign->setTitle(QApplication::translate("displayWords", "myoSign", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class displayWords: public Ui_displayWords {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISPLAYWORDS_H
