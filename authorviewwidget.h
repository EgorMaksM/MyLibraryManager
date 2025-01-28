#ifndef AUTHORVIEWWIDGET_H
#define AUTHORVIEWWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

#include "smoothscrollarea.h"
#include "author.h"

namespace Ui {
class AuthorViewWidget;
}

class AuthorViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AuthorViewWidget(QWidget *parent = nullptr);
    ~AuthorViewWidget();

    void openAuthor(Author& author);

private:
    Ui::AuthorViewWidget *ui;

    QVBoxLayout* mainLayout;
    SmoothScrollArea* scrollArea;
    QWidget* scrollContent;
    QVBoxLayout* scrollContentLayout;

    QHBoxLayout* nameLayout;
    QLabel* nameFieldLabel;
    QLabel* nameValueLabel;

    QHBoxLayout* lifetimeLayout;
    QLabel* lifetimeFieldLabel;
    QLabel* lifetimeValueLabel;

    QHBoxLayout* IDLayout;
    QLabel* IDFieldLabel;
    QLabel* IDValueLabel;

    QHBoxLayout* bioLayout;
    QLabel* bioFieldLabel;
    QLabel* bioValueLabel;


};

#endif // AUTHORVIEWWIDGET_H
