#include "authorviewwidget.h"
#include "ui_authorviewwidget.h"

AuthorViewWidget::AuthorViewWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AuthorViewWidget)
{
    ui->setupUi(this);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    mainLayout = new QVBoxLayout(this);

    scrollArea = new SmoothScrollArea(this);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    scrollContent = new QWidget();
    scrollContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scrollArea->setWidget(scrollContent);
    scrollArea->setWidgetResizable(true);
    scrollContentLayout = new QVBoxLayout(scrollContent);
    scrollContent->setLayout(scrollContentLayout);

    // Name Field
    nameLayout = new QHBoxLayout();
    nameFieldLabel = new QLabel(this);
    nameValueLabel = new QLabel(this);
    nameFieldLabel->setText("Name:");
    nameValueLabel->setText("-");
    nameLayout->addWidget(nameFieldLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);
    nameLayout->addStretch();
    nameLayout->addWidget(nameValueLabel, 0, Qt::AlignRight | Qt::AlignVCenter);
    scrollContentLayout->addLayout(nameLayout);

    // Lifetime Field
    lifetimeLayout = new QHBoxLayout();
    lifetimeFieldLabel = new QLabel(this);
    lifetimeValueLabel = new QLabel(this);
    lifetimeFieldLabel->setText("Lifetime:");
    lifetimeValueLabel->setText("-");
    lifetimeLayout->addWidget(lifetimeFieldLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);
    lifetimeLayout->addStretch();
    lifetimeLayout->addWidget(lifetimeValueLabel, 0, Qt::AlignRight | Qt::AlignVCenter);
    scrollContentLayout->addLayout(lifetimeLayout);

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

    // Bio Field
    bioLayout = new QHBoxLayout();
    bioFieldLabel = new QLabel(this);
    bioValueLabel = new QLabel(this);
    bioFieldLabel->setText("Bio:");
    bioValueLabel->setText("-");
    bioValueLabel->setWordWrap(true);
    bioValueLabel->setAlignment(Qt::AlignRight | Qt::AlignHCenter);
    bioLayout->addWidget(bioFieldLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);
    bioLayout->addStretch();
    bioLayout->addWidget(bioValueLabel, 0, Qt::AlignRight | Qt::AlignVCenter);
    scrollContentLayout->addLayout(bioLayout);


    scrollContentLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    mainLayout->addWidget(scrollArea);

    this->setLayout(mainLayout);
}

AuthorViewWidget::~AuthorViewWidget()
{
    delete ui;
}

void AuthorViewWidget::openAuthorByID(sqlite3*& DB, int author_id) {
    Author author;
    bool bCorrectID = getAuthorByID(DB, author_id, author);
    if (bCorrectID) {
        // Display Name
        nameValueLabel->setText(author.forename + " " + author.surname);

        // Display Lifetime
        if (author.death.isValid())
        lifetimeValueLabel->setText(QLocale().toString(author.birth, "MMMM d, yyyy") + "-" + QLocale().toString(author.death, "MMMM d, yyyy"));
        else
            lifetimeValueLabel->setText(QLocale().toString(author.birth, "MMMM d, yyyy") + " - Present day");

        // Display ID
        IDValueLabel->setText(QString::number(author.id));

        // Display Biography
        bioValueLabel->setText(author.bio);
    }
    else {
        qDebug() << "Error: Incorrect Author ID!";
    }
}

