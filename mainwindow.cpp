#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(sqlite3*& DB, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , DB(DB)
{
    ui->setupUi(this);

    centralWidget = new QSplitter(this);
    browsingWidget = new QSplitter(Qt::Vertical);
    tableWidget = new TableViewWidget(DB, this);
    controlsLayout = new QVBoxLayout();
    controlsLayoutWidget = new QWidget(this);
    stackedWidget = new QStackedWidget(this);
    bookView = new BookViewWidget(this);
    authorView = new AuthorViewWidget(this);
    userView = new UserViewWidget(this);
    noObjectChosen = new QLabel(this);
    noObjectChosen->setAlignment(Qt::AlignCenter);

    this->setCentralWidget(centralWidget);
    centralWidget->addWidget(browsingWidget);
    centralWidget->addWidget(stackedWidget);
    centralWidget->setSizes({1480, 440});


    // Controls
    controlsLayoutWidget->setLayout(controlsLayout);
    browsingWidget->addWidget(controlsLayoutWidget);
    controlsLayoutWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setupNewBtn();
    browsingWidget->setCollapsible(0, false);

    // Table
    browsingWidget->addWidget(tableWidget);
    connect(tableWidget->gridWidget, &QTableWidget::cellDoubleClicked, this, &MainWindow::onObjectSelected);
    browsingWidget->setSizes({20, 1000});
    browsingWidget->setCollapsible(1, false);


    // Selection Overview
    centralWidget->setCollapsible(0, false);
    centralWidget->setCollapsible(1, false);
    QWidget* container = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(container);
    container->setLayout(layout);

    layout->addWidget(noObjectChosen, Qt::AlignCenter);
    stackedWidget->addWidget(container);
    stackedWidget->addWidget(bookView);
    stackedWidget->addWidget(authorView);
    stackedWidget->addWidget(userView);

    noObjectChosen->setText("Nothing is selected! Select an object to inspect it.");
    noObjectChosen->setWordWrap(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onObjectSelected(int row, int column) {
    Q_UNUSED(column);
    switch (tableWidget->curObjectType) {
    case TableViewWidget::BOOK: {
        int id = tableWidget->gridWidget->item(row, 0)->data(Qt::UserRole).toInt();
        QString title = tableWidget->gridWidget->item(row, 1)->data(Qt::UserRole).toString();
        int year = tableWidget->gridWidget->item(row, 2)->data(Qt::UserRole).toInt();
        Book book(id, title, year);
        bookView->openBook(DB, book);
        stackedWidget->setCurrentWidget(bookView);
        break;
        }
    case TableViewWidget::AUTHOR: {
        int id = tableWidget->gridWidget->item(row, 0)->data(Qt::UserRole).toInt();
        QString forename = tableWidget->gridWidget->item(row, 1)->data(Qt::UserRole).toString();
        QString surname = tableWidget->gridWidget->item(row, 2)->data(Qt::UserRole).toString();
        QString bio = tableWidget->gridWidget->item(row, 3)->data(Qt::UserRole).toString();
        QDate birth = tableWidget->gridWidget->item(row, 4)->data(Qt::UserRole).toDate();
        QDate death = tableWidget->gridWidget->item(row, 5)->data(Qt::UserRole).toDate();
        Author author(id, forename, surname, bio, birth, death);
        authorView->openAuthor(author);
        stackedWidget->setCurrentWidget(authorView);
        break;
        }
    case TableViewWidget::USER: {
        int id = tableWidget->gridWidget->item(row, 0)->data(Qt::UserRole).toInt();
        QString forename = tableWidget->gridWidget->item(row, 1)->data(Qt::UserRole).toString();
        QString surname = tableWidget->gridWidget->item(row, 2)->data(Qt::UserRole).toString();
        QDate birth = tableWidget->gridWidget->item(row, 3)->data(Qt::UserRole).toDate();
        QString email = tableWidget->gridWidget->item(row, 4)->data(Qt::UserRole).toString();
        QString phone = tableWidget->gridWidget->item(row, 5)->data(Qt::UserRole).toString();
        User user(id, forename, surname, birth, email, phone);
        userView->openUser(user);
        stackedWidget->setCurrentWidget(userView);
        break;
        }
    default:
            qDebug() << "Idk. Look into MainWindow::onObjectSelected";
    }
}

void MainWindow::setupNewBtn() {
    QPushButton* dropdownButton = new QPushButton("+", this);
    dropdownButton->setFixedSize(35, 35);
    dropdownButton->setStyleSheet(
        "QPushButton::menu-indicator { width: 0px; height: 0px; }"
        "QPushButton { text-align: center; padding: 0px; }"
        );

    QMenu* menu = new QMenu(dropdownButton);
    menu->setStyleSheet(
        "QMenu {"
        "    padding: 0px;"
        "    margin: 0px;"
        "    border: 1px solid #dcdcdc;"
        "    background-color: #212121;"
        "    border-radius: 0px;"
        "}"
        "QMenu::item {"
        "    padding: 4px 8px;"
        "    margin: 0px;"
        "}"
        "QMenu::item:selected {"
        "    background-color: #303030;" // Optional: hover effect
        "}"
        );

    QAction* option1 = menu->addAction("New Book");
    QAction* option2 = menu->addAction("New Author");
    QAction* option3 = menu->addAction("New User");

    connect(option1, &QAction::triggered, this, [this]() { openDialog(0); });
    connect(option2, &QAction::triggered, this, [this]() { openDialog(1); });
    connect(option3, &QAction::triggered, this, [this]() { openDialog(2); });

    dropdownButton->setMenu(menu);

    controlsLayout->addWidget(dropdownButton, 0, Qt::AlignLeft | Qt::AlignTop);
}

void MainWindow::openDialog(int objectType) {  // 0 -> Book; 1 -> Author; 2 -> User
    if (objectType == 0) {
        AddBookDialog* dialog = new AddBookDialog(this);
        connect(dialog, &AddBookDialog::bookSubmitted, this, [this](const QString& title, int year){
            addBook(DB, title, year);
        });
        dialog->exec();
        //std::vector<Book> books = getBooks(DB);
        //tableWidget->populateTable(books);
    }
}
