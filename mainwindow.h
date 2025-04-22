#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QSplitter>
#include <QStackedWidget>
#include <QPushButton>

#include "bookviewwidget.h"
#include "authorviewwidget.h"
#include "userviewwidget.h"
#include "tableviewwidget.h"
#include "addbookdialog.h"

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
    QVBoxLayout* controlsLayout;
    QWidget* controlsLayoutWidget;

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

    void onObjectSelected(int row, int column);

    void setupNewBtn();
    void openDialog(int objectType); // 0 -> Book; 1 -> Author; 2 -> User

};
#endif // MAINWINDOW_H
