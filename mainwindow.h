#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QSplitter>

#include "bookviewwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QSplitter* centralWidget;
    QVBoxLayout* mainLayout;
    QWidget* mainLayoutWidget;

    BookViewWidget* bookView;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
