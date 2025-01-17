#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    centralWidget = new QSplitter(this);
    mainLayout = new QVBoxLayout();
    mainLayoutWidget = new QWidget(this);
    bookView = new BookViewWidget(this);

    QLabel* lab = new QLabel(this);
    lab->setText("Text");

    mainLayoutWidget->setLayout(mainLayout);
    mainLayout->addWidget(lab);
    this->setCentralWidget(centralWidget);
    centralWidget->addWidget(mainLayoutWidget);
    centralWidget->addWidget(bookView);
    centralWidget->setSizes({1480, 440});

    lab->setStyleSheet("background-color: white;");
}

MainWindow::~MainWindow()
{
    delete ui;
}
