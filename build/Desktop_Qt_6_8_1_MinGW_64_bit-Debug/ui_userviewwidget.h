/********************************************************************************
** Form generated from reading UI file 'userviewwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERVIEWWIDGET_H
#define UI_USERVIEWWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserViewWidget
{
public:

    void setupUi(QWidget *UserViewWidget)
    {
        if (UserViewWidget->objectName().isEmpty())
            UserViewWidget->setObjectName("UserViewWidget");
        UserViewWidget->resize(400, 300);

        retranslateUi(UserViewWidget);

        QMetaObject::connectSlotsByName(UserViewWidget);
    } // setupUi

    void retranslateUi(QWidget *UserViewWidget)
    {
        UserViewWidget->setWindowTitle(QCoreApplication::translate("UserViewWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UserViewWidget: public Ui_UserViewWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERVIEWWIDGET_H
