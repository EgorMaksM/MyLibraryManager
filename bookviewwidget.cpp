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
