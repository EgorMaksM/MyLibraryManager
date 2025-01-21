#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(sqlite3*& DB, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , DB(DB)
{
    ui->setupUi(this);

    centralWidget = new QSplitter(this);
    mainLayout = new QVBoxLayout();
    mainLayoutWidget = new QWidget(this);
    bookView = new BookViewWidget(this);
    authorView = new AuthorViewWidget(this);
    userView = new UserViewWidget(this);
    noObjectChosen = new QLabel(this);
    noObjectChosen->setAlignment(Qt::AlignCenter);
    stackedWidget = new QStackedWidget(this);

    QLabel* lab = new QLabel(this);
    lab->setText("Text");

    mainLayoutWidget->setLayout(mainLayout);
    mainLayout->addWidget(lab);
    this->setCentralWidget(centralWidget);
    centralWidget->addWidget(mainLayoutWidget);
    centralWidget->addWidget(stackedWidget);
    centralWidget->setSizes({1480, 440});

    lab->setStyleSheet("background-color: white;");

    QWidget* container = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(this);
    container->setLayout(layout);
    layout->addWidget(noObjectChosen, Qt::AlignCenter);

    stackedWidget->addWidget(container);
    stackedWidget->addWidget(bookView);
    stackedWidget->addWidget(authorView);
    stackedWidget->addWidget(userView);

    userView->openUserByID(DB, 1);
    bookView->openBookByID(DB, 1);
    authorView->openAuthorByID(DB, 1);
    noObjectChosen->setText("Nothing is selected! Select an object to inspect it.");
}

MainWindow::~MainWindow()
{
    delete ui;
}
