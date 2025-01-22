/********************************************************************************
** Form generated from reading UI file 'smoothscrollarea.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SMOOTHSCROLLAREA_H
#define UI_SMOOTHSCROLLAREA_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SmoothScrollArea
{
public:

    void setupUi(QWidget *SmoothScrollArea)
    {
        if (SmoothScrollArea->objectName().isEmpty())
            SmoothScrollArea->setObjectName("SmoothScrollArea");
        SmoothScrollArea->resize(400, 300);

        retranslateUi(SmoothScrollArea);

        QMetaObject::connectSlotsByName(SmoothScrollArea);
    } // setupUi

    void retranslateUi(QWidget *SmoothScrollArea)
    {
        SmoothScrollArea->setWindowTitle(QCoreApplication::translate("SmoothScrollArea", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SmoothScrollArea: public Ui_SmoothScrollArea {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SMOOTHSCROLLAREA_H
