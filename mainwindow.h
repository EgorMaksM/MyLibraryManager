#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QSplitter>
#include <QStackedWidget>

#include "bookviewwidget.h"
#include "authorviewwidget.h"
#include "userviewwidget.h"
#include "tableviewwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(sqlite3*& DB, QWidget *parent = nullptr);
    ~MainWindow();

    QSplitter* centralWidget;
    QVBoxLayout* mainLayout;
    QWidget* mainLayoutWidget;

    QSplitter* browsingWidget;
    TableViewWidget* tableWidget;

    QStackedWidget* stackedWidget;
    QLabel* noObjectChosen;
    BookViewWidget* bookView;
    AuthorViewWidget* authorView;
    UserViewWidget* userView;

private:
    Ui::MainWindow *ui;
    sqlite3*& DB;
};
#endif // MAINWINDOW_H
