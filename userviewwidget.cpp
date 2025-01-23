#include "userviewwidget.h"
#include "ui_userviewwidget.h"

UserViewWidget::UserViewWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UserViewWidget)
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

    // Birth Field
    birthLayout = new QHBoxLayout();
    birthFieldLabel = new QLabel(this);
    birthValueLabel = new QLabel(this);
    birthFieldLabel->setText("Birth Date:");
    birthValueLabel->setText("-");
    birthLayout->addWidget(birthFieldLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);
    birthLayout->addStretch();
    birthLayout->addWidget(birthValueLabel, 0, Qt::AlignRight | Qt::AlignVCenter);
    scrollContentLayout->addLayout(birthLayout);

    // Email Field
    emailLayout = new QHBoxLayout();
    emailFieldLabel = new QLabel(this);
    emailValueLabel = new QLabel(this);
    emailFieldLabel->setText("Email:");
    emailValueLabel->setText("-");
    emailValueLabel->setWordWrap(true);
    emailValueLabel->setAlignment(Qt::AlignRight | Qt::AlignHCenter);
    emailLayout->addWidget(emailFieldLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);
    emailLayout->addStretch();
    emailLayout->addWidget(emailValueLabel, 0, Qt::AlignRight | Qt::AlignVCenter);
    scrollContentLayout->addLayout(emailLayout);

    // Phonenumber Field
    phoneLayout = new QHBoxLayout();
    phoneFieldLabel = new QLabel(this);
    phoneValueLabel = new QLabel(this);
    phoneFieldLabel->setText("Phone Number:");
    phoneValueLabel->setText("-");
    phoneValueLabel->setWordWrap(true);
    phoneValueLabel->setAlignment(Qt::AlignRight | Qt::AlignHCenter);
    phoneLayout->addWidget(phoneFieldLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);
    phoneLayout->addStretch();
    phoneLayout->addWidget(phoneValueLabel, 0, Qt::AlignRight | Qt::AlignVCenter);
    scrollContentLayout->addLayout(phoneLayout);

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

UserViewWidget::~UserViewWidget()
{
    delete ui;
}

void UserViewWidget::openUserByID(sqlite3*& DB, int user_id) {
    User user;
    bool bCorrectID = getUserByID(DB, user_id, user);
    if (bCorrectID) {
        // Display Name
        nameValueLabel->setText(user.forename + " " + user.surname);

        // Display Birthdate
        QDate today = QDate::currentDate();
        int years = today.year() - user.birth.year();
        if ((today.month() * 30 + today.day()) < (user.birth.month() * 30 + user.birth.day())) years -= 1;
        birthValueLabel->setText(QLocale().toString(user.birth, "MMMM d, yyyy") + " (" + QString::number(years) + " years)");

        // Display Email
        emailValueLabel->setText(user.email);

        // Display Phone
        if (user.phone.isEmpty())
            phoneValueLabel->setText(QString::fromStdString("No phone number"));
        else
            phoneValueLabel->setText(user.phone);

        // Display ID
        IDValueLabel->setText(QString::number(user.id));
    }
    else {
        qDebug() << "Error: Incorrect Author ID!";
    }
}

int UserViewWidget::getYears(const QDate& date) {
    QDate today = QDate::currentDate();
    int years = today.year() - date.year();
    if ((today.month() * 30 + today.day()) < (date.month() * 30 + date.day())) years -= 1;
    return years;
}
