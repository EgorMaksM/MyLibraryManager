/********************************************************************************
** Form generated from reading UI file 'tableviewwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TABLEVIEWWIDGET_H
#define UI_TABLEVIEWWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TableViewWidget
{
public:

    void setupUi(QWidget *TableViewWidget)
    {
        if (TableViewWidget->objectName().isEmpty())
            TableViewWidget->setObjectName("TableViewWidget");
        TableViewWidget->resize(400, 300);

        retranslateUi(TableViewWidget);

        QMetaObject::connectSlotsByName(TableViewWidget);
    } // setupUi

    void retranslateUi(QWidget *TableViewWidget)
    {
        TableViewWidget->setWindowTitle(QCoreApplication::translate("TableViewWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TableViewWidget: public Ui_TableViewWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TABLEVIEWWIDGET_H
