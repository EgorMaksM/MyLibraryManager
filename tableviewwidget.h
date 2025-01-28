#ifndef TABLEVIEWWIDGET_H
#define TABLEVIEWWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QVBoxLayout>
#include <QStringList>

#include "libmgr.h"

namespace Ui {
class TableViewWidget;
}

class TableViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TableViewWidget(sqlite3*& DB, QWidget *parent = nullptr);
    ~TableViewWidget();

    void populateTable(std::vector<Book> books);
    void populateTable(std::vector<Author> authors);
    void populateTable(std::vector<User> users);

    enum TableFilling {
        BOOK,
        AUTHOR,
        USER
    };

    TableFilling curObjectType;
    QTableWidget* gridWidget;

private:
    Ui::TableViewWidget *ui;

    sqlite3*& DB;
};

#endif // TABLEVIEWWIDGET_H
