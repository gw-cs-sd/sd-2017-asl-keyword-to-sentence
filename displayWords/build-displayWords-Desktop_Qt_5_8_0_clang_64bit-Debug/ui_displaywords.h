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
    QLabel *displayTheWord;
    QMenuBar *menuBar;
    QMenu *menumyoSign;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *displayWords)
    {
        if (displayWords->objectName().isEmpty())
            displayWords->setObjectName(QStringLiteral("displayWords"));
        displayWords->resize(400, 300);
        centralWidget = new QWidget(displayWords);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 10, 381, 221));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        displayTheWord = new QLabel(verticalLayoutWidget);
        displayTheWord->setObjectName(QStringLiteral("displayTheWord"));
        QFont font;
        font.setPointSize(30);
        displayTheWord->setFont(font);
        displayTheWord->setAutoFillBackground(true);
        displayTheWord->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(displayTheWord);

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
        displayTheWord->setText(QApplication::translate("displayWords", "TextLabel", Q_NULLPTR));
        menumyoSign->setTitle(QApplication::translate("displayWords", "myoSign", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class displayWords: public Ui_displayWords {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISPLAYWORDS_H
