#ifndef BOOKVIEWWIDGET_H
#define BOOKVIEWWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>

#include "libmgr.h"

namespace Ui {
class BookViewWidget;
}

class BookViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BookViewWidget(QWidget *parent = nullptr);
    ~BookViewWidget();

    bool openBookByID(sqlite3*& DB, int book_id);

private:
    Ui::BookViewWidget *ui;

    QVBoxLayout* mainLayout;
    QScrollArea* scrollArea;
    QWidget* scrollContent;
    QVBoxLayout* scrollContentLayout;

    QHBoxLayout* titleLayout;
    QLabel* titleFieldLabel;
    QLabel* titleValueLabel;

    QHBoxLayout* authorLayout;
    QLabel* authorFieldLabel;
    QLabel* authorValueLabel;

    QHBoxLayout* yearLayout;
    QLabel* yearFieldLabel;
    QLabel* yearValueLabel;

    QHBoxLayout* IDLayout;
    QLabel* IDFieldLabel;
    QLabel* IDValueLabel;
};

#endif // BOOKVIEWWIDGET_H
