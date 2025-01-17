/********************************************************************************
** Form generated from reading UI file 'bookviewwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BOOKVIEWWIDGET_H
#define UI_BOOKVIEWWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BookViewWidget
{
public:

    void setupUi(QWidget *BookViewWidget)
    {
        if (BookViewWidget->objectName().isEmpty())
            BookViewWidget->setObjectName("BookViewWidget");
        BookViewWidget->resize(440, 1000);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(BookViewWidget->sizePolicy().hasHeightForWidth());
        BookViewWidget->setSizePolicy(sizePolicy);

        retranslateUi(BookViewWidget);

        QMetaObject::connectSlotsByName(BookViewWidget);
    } // setupUi

    void retranslateUi(QWidget *BookViewWidget)
    {
        BookViewWidget->setWindowTitle(QCoreApplication::translate("BookViewWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BookViewWidget: public Ui_BookViewWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BOOKVIEWWIDGET_H
