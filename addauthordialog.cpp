#include "addauthordialog.h"
#include "ui_addauthordialog.h"

AddAuthorDialog::AddAuthorDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddAuthorDialog)
{
    ui->setupUi(this);

    setWindowTitle("Add New Author");
    setModal(true);
    setFixedSize(600, 450);

    // Create input fields
    forenameInput = new QLineEdit(this);
    forenameInput->setPlaceholderText("Enter The Author's Forename");

    surnameInput = new QLineEdit(this);
    surnameInput->setPlaceholderText("Enter The Author's Surname");

    bioInput = new QTextEdit(this);
    bioInput->setPlaceholderText("Enter The Author's Biography");

    birthDateInput = new QDateEdit(this);
    birthDateInput->setCalendarPopup(true);
    birthDateInput->setDate(QDate::currentDate());

    deathDateInput = new QDateEdit(this);
    deathDateInput->setCalendarPopup(true);
    deathDateInput->setDate(QDate::currentDate());

    QPushButton* submitButton = new QPushButton("Submit", this);
    connect(submitButton, &QPushButton::clicked, this, &AddAuthorDialog::onSubmitClicked);

    // Layout
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(forenameInput);
    layout->addWidget(surnameInput);
    layout->addWidget(bioInput);
    layout->addWidget(birthDateInput);
    layout->addWidget(deathDateInput);
    layout->addWidget(submitButton);

    setLayout(layout);
}

void AddAuthorDialog::onSubmitClicked() {
    QString forename = forenameInput->text();
    QString surname = surnameInput->text();
    QString bio = bioInput->toPlainText();
    QDate birth = birthDateInput->date();
    QDate death = deathDateInput->date();
    emit authorSubmitted(forename, surname, bio, birth, death);
    this->accept();
}

AddAuthorDialog::~AddAuthorDialog()
{
    delete ui;
}
