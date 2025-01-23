/********************************************************************************
** Form generated from reading UI file 'authorviewwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUTHORVIEWWIDGET_H
#define UI_AUTHORVIEWWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AuthorViewWidget
{
public:

    void setupUi(QWidget *AuthorViewWidget)
    {
        if (AuthorViewWidget->objectName().isEmpty())
            AuthorViewWidget->setObjectName("AuthorViewWidget");
        AuthorViewWidget->resize(724, 550);

        retranslateUi(AuthorViewWidget);

        QMetaObject::connectSlotsByName(AuthorViewWidget);
    } // setupUi

    void retranslateUi(QWidget *AuthorViewWidget)
    {
        AuthorViewWidget->setWindowTitle(QCoreApplication::translate("AuthorViewWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AuthorViewWidget: public Ui_AuthorViewWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUTHORVIEWWIDGET_H
