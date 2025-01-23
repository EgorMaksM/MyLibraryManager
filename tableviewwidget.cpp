#include "tableviewwidget.h"
#include "ui_tableviewwidget.h"

TableViewWidget::TableViewWidget(sqlite3*& DB, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TableViewWidget)
    , DB(DB)
{
    ui->setupUi(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);


    gridWidget = new QTableWidget(this);
    gridWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(gridWidget);

    // Table style setup
    gridWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    gridWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    gridWidget->setSelectionMode(QAbstractItemView::NoSelection);
    gridWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    gridWidget->verticalHeader()->setVisible(false);
    gridWidget->horizontalHeader()->setSortIndicatorShown(true);
    gridWidget->setSortingEnabled(true);

    gridWidget->setStyleSheet(R"(
    QTableWidget {
        border: 1px solid #DFDFDF;
        border-radius: 0px;
    }

    QHeaderView::section {
        border: 1px solid #DFDFDF;
        border-radius: 0px;
        padding-right: 20px;
    }
    QHeaderView::section:first-child {
        text-align: center; /* Align text to the center */
    }
)");

    //std::vector<Book> books = sortBooks(DB, ID);
    std::vector<Author> authors = getAuthors(DB);
    std::vector<User> users = getUsers(DB);
    populateTable(authors);
}

TableViewWidget::~TableViewWidget()
{
    delete ui;
}

void TableViewWidget::populateTable(std::vector<Book> books) {
    const int rows = books.size();
    const int cols = 4;

    gridWidget->setRowCount(rows);
    gridWidget->setColumnCount(cols);

    gridWidget->blockSignals(true);

    QStringList headers;
    headers << "ID" << "Title" << "Year of publication" << "Author(s)";
    gridWidget->setHorizontalHeaderLabels(headers);
    for (int i = 0; i < rows; i++) {
        QTableWidgetItem* id = new QTableWidgetItem(QString::number(books[i].id));
        id->setData(Qt::UserRole, books[i].id);
        id->setTextAlignment(Qt::AlignHCenter);
        gridWidget->setItem(i, 0, id);
        QTableWidgetItem* title = new QTableWidgetItem(books[i].title);
        gridWidget->setItem(i, 1, title);
        QTableWidgetItem* year = new QTableWidgetItem(QString::number(books[i].year));
        year->setData(Qt::UserRole, books[i].year);
        gridWidget->setItem(i, 2, year);

        std::vector<Author> resultAuthors;
        getAuthorsByBookID(DB, books[i].id, resultAuthors);

        if (resultAuthors.size() > 1) {
            QString authorsList;
            for (unsigned long long i = 0; i < resultAuthors.size() - 1; i++) {
                authorsList += resultAuthors[i].forename + " " + resultAuthors[i].surname = ", ";
            }
            authorsList += resultAuthors.back().forename + " " + resultAuthors.back().surname;
            QTableWidgetItem* authors = new QTableWidgetItem(authorsList);
            gridWidget->setItem(i, 3, authors);
        }
        else {
            QString author_str = resultAuthors[0].forename + " " + resultAuthors[0].surname;
            QTableWidgetItem* author = new QTableWidgetItem(author_str);
            gridWidget->setItem(i, 3, author);
        }
    }

    gridWidget->blockSignals(false);
    gridWidget->resizeColumnsToContents();
}

void TableViewWidget::populateTable(std::vector<Author> authors) {
    const int rows = authors.size();
    const int cols = 5;

    qDebug() << rows;

    gridWidget->setRowCount(rows);
    gridWidget->setColumnCount(cols);

    gridWidget->blockSignals(true);

    QStringList headers;
    headers << "ID" << "Forename" << "Surname" << "Date of Birth" << "Date of Death";
    gridWidget->setHorizontalHeaderLabels(headers);
    for (int i = 0; i < rows; i++) {
        QTableWidgetItem* id = new QTableWidgetItem(QString::number(authors[i].id));
        id->setData(Qt::UserRole, authors[i].id);
        id->setTextAlignment(Qt::AlignHCenter);
        gridWidget->setItem(i, 0, id);
        QTableWidgetItem* forename = new QTableWidgetItem(authors[i].forename);
        gridWidget->setItem(i, 1, forename);
        QTableWidgetItem* surname = new QTableWidgetItem(authors[i].surname);
        gridWidget->setItem(i, 2, surname);
        QTableWidgetItem* birth = new QTableWidgetItem(QLocale().toString(authors[i].birth, "MMM d, yyyy"));
        birth->setData(Qt::UserRole, authors[i].birth);
        gridWidget->setItem(i, 3, birth);
        QTableWidgetItem* death;
        if (authors[i].death.isValid())
            death = new QTableWidgetItem(QLocale().toString(authors[i].death, "MMM d, yyyy"));
        else
            death = new QTableWidgetItem("Living");
        birth->setData(Qt::UserRole, authors[i].death);
        gridWidget->setItem(i, 4, death);
    }

    gridWidget->blockSignals(false);
    gridWidget->resizeColumnsToContents();
}

void TableViewWidget::populateTable(std::vector<User> users) {
    const int rows = users.size();
    const int cols = 6;

    qDebug() << rows;

    gridWidget->setRowCount(rows);
    gridWidget->setColumnCount(cols);

    gridWidget->blockSignals(true);

    QStringList headers;
    headers << "ID" << "Forename" << "Surname" << "Date of Birth" << "Email" << "Phone Number";
    gridWidget->setHorizontalHeaderLabels(headers);
    for (int i = 0; i < rows; i++) {
        QTableWidgetItem* id = new QTableWidgetItem(QString::number(users[i].id));
        id->setData(Qt::UserRole, users[i].id);
        id->setTextAlignment(Qt::AlignHCenter);
        gridWidget->setItem(i, 0, id);
        QTableWidgetItem* forename = new QTableWidgetItem(users[i].forename);
        gridWidget->setItem(i, 1, forename);
        QTableWidgetItem* surname = new QTableWidgetItem(users[i].surname);
        gridWidget->setItem(i, 2, surname);
        QTableWidgetItem* birth = new QTableWidgetItem(QLocale().toString(users[i].birth, "MMM d, yyyy"));
        birth->setData(Qt::UserRole, users[i].birth);
        gridWidget->setItem(i, 3, birth);
        QTableWidgetItem* email = new QTableWidgetItem(users[i].email);
        gridWidget->setItem(i, 4, email);
        QTableWidgetItem* phone = new QTableWidgetItem(users[i].phone);
        gridWidget->setItem(i, 5, phone);
    }

    gridWidget->blockSignals(false);
    gridWidget->resizeColumnsToContents();
}
