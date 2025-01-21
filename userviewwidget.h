#ifndef USERVIEWWIDGET_H
#define USERVIEWWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QDate>

#include "libmgr.h"

namespace Ui {
class UserViewWidget;
}

class UserViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserViewWidget(QWidget *parent = nullptr);
    ~UserViewWidget();

    void openUserByID(sqlite3*& DB, int user_id);

private:
    Ui::UserViewWidget *ui;

    QVBoxLayout* mainLayout;
    QScrollArea* scrollArea;
    QWidget* scrollContent;
    QVBoxLayout* scrollContentLayout;

    QHBoxLayout* nameLayout;
    QLabel* nameFieldLabel;
    QLabel* nameValueLabel;

    QHBoxLayout* birthLayout;
    QLabel* birthFieldLabel;
    QLabel* birthValueLabel;

    QHBoxLayout* emailLayout;
    QLabel* emailFieldLabel;
    QLabel* emailValueLabel;

    QHBoxLayout* phoneLayout;
    QLabel* phoneFieldLabel;
    QLabel* phoneValueLabel;

    QHBoxLayout* IDLayout;
    QLabel* IDFieldLabel;
    QLabel* IDValueLabel;

    int getYears(const QDate& date);
};

#endif // USERVIEWWIDGET_H
