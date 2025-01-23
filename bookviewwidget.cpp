#include "bookviewwidget.h"
#include "ui_bookviewwidget.h"

BookViewWidget::BookViewWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BookViewWidget)
{
    ui->setupUi(this);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    mainLayout = new QVBoxLayout(this);

    scrollArea = new QScrollArea(this);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    scrollContent = new QWidget();
    scrollContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scrollArea->setWidget(scrollContent);
    scrollArea->setWidgetResizable(true);
    scrollContentLayout = new QVBoxLayout(scrollContent);
    scrollContent->setLayout(scrollContentLayout);

    // Title Field
    titleLayout = new QHBoxLayout();
    titleFieldLabel = new QLabel(this);
    titleValueLabel = new QLabel(this);
    titleFieldLabel->setText("Title:");
    titleValueLabel->setText("-");
    titleLayout->addWidget(titleFieldLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);
    titleLayout->addStretch();
    titleLayout->addWidget(titleValueLabel, 0, Qt::AlignRight | Qt::AlignVCenter);
    scrollContentLayout->addLayout(titleLayout);

    // Author Field
    authorLayout = new QHBoxLayout();
    authorFieldLabel = new QLabel(this);
    authorValueLabel = new QLabel(this);
    authorFieldLabel->setText("Author:");
    authorValueLabel->setText("-");
    authorLayout->addWidget(authorFieldLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);
    authorLayout->addStretch();
    authorLayout->addWidget(authorValueLabel, 0, Qt::AlignRight | Qt::AlignVCenter);
    scrollContentLayout->addLayout(authorLayout);

    // Year Field
    yearLayout = new QHBoxLayout();
    yearFieldLabel = new QLabel(this);
    yearValueLabel = new QLabel(this);
    yearFieldLabel->setText("Year:");
    yearValueLabel->setText("-");
    yearLayout->addWidget(yearFieldLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);
    yearLayout->addStretch();
    yearLayout->addWidget(yearValueLabel, 0, Qt::AlignRight | Qt::AlignVCenter);
    scrollContentLayout->addLayout(yearLayout);

    // ID Field
    IDLayout = new QHBoxLayout();
    IDFieldLabel = new QLabel(this);
    IDValueLabel = new QLabel(this);
    IDFieldLabel->setText("ID:");
    IDValueLabel->setText("-");
    IDLayout->addWidget(IDFieldLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);
    IDLayout->addStretch();
    IDLayout->addWidget(IDValueLabel, 0, Qt::AlignRight | Qt::AlignVCenter);
    scrollContentLayout->addLayout(IDLayout);

    scrollContentLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    mainLayout->addWidget(scrollArea);

    this->setLayout(mainLayout);
}

BookViewWidget::~BookViewWidget()
{
    delete ui;
}

void BookViewWidget::openBookByID(sqlite3*& DB, int book_id) {
    Book book;
    bool bCorrectID = getBookByID(DB, book_id, book);
    if (bCorrectID) {
        // Display title
        titleValueLabel->setText(book.title);

        // Display author(s)
        std::vector<Author> resultAuthors;
        getAuthorsByBookID(DB, book_id, resultAuthors);

        if (resultAuthors.size() > 1) {
            authorFieldLabel->setText("Authors:");
            QString newAuthorsList;
            for (unsigned long long i = 0; i < resultAuthors.size() - 1; i++) {
                newAuthorsList += resultAuthors[i].forename + " " + resultAuthors[i].surname = ", ";
            }
            newAuthorsList += resultAuthors.back().forename + " " + resultAuthors.back().surname;
            authorValueLabel->setText(newAuthorsList);
        }
        else {
            authorFieldLabel->setText("Author:");
            authorValueLabel->setText(resultAuthors[0].forename + " " + resultAuthors[0].surname);
        }

        // Display year
        yearValueLabel->setText(QString::number(book.year));
        // Display ID
        IDValueLabel->setText(QString::number(book_id));
    }
    else {
        qDebug() << "Error: Incorrect Book ID!";
    }
}
