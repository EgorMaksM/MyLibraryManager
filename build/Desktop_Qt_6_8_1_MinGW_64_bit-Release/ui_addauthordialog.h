/********************************************************************************
** Form generated from reading UI file 'addauthordialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDAUTHORDIALOG_H
#define UI_ADDAUTHORDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_AddAuthorDialog
{
public:

    void setupUi(QDialog *AddAuthorDialog)
    {
        if (AddAuthorDialog->objectName().isEmpty())
            AddAuthorDialog->setObjectName("AddAuthorDialog");
        AddAuthorDialog->resize(400, 300);

        retranslateUi(AddAuthorDialog);

        QMetaObject::connectSlotsByName(AddAuthorDialog);
    } // setupUi

    void retranslateUi(QDialog *AddAuthorDialog)
    {
        AddAuthorDialog->setWindowTitle(QCoreApplication::translate("AddAuthorDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AddAuthorDialog: public Ui_AddAuthorDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDAUTHORDIALOG_H
