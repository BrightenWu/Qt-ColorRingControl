/********************************************************************************
** Form generated from reading UI file 'colorctrltest.ui'
**
** Created by: Qt User Interface Compiler version 5.9.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COLORCTRLTEST_H
#define UI_COLORCTRLTEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_colorctrltestClass
{
public:

    void setupUi(QWidget *colorctrltestClass)
    {
        if (colorctrltestClass->objectName().isEmpty())
            colorctrltestClass->setObjectName(QStringLiteral("colorctrltestClass"));
        colorctrltestClass->resize(600, 400);

        retranslateUi(colorctrltestClass);

        QMetaObject::connectSlotsByName(colorctrltestClass);
    } // setupUi

    void retranslateUi(QWidget *colorctrltestClass)
    {
        colorctrltestClass->setWindowTitle(QApplication::translate("colorctrltestClass", "colorctrltest", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class colorctrltestClass: public Ui_colorctrltestClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COLORCTRLTEST_H
