#include "addbookdialog.h"
#include "ui_addbookdialog.h"

AddBookDialog::AddBookDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddBookDialog)
{
    ui->setupUi(this);

    setWindowTitle("Add New Book");
    setModal(true); // Make it modal
    setFixedSize(300, 150); // Fixed size

    // Create input fields
    titleInput = new QLineEdit(this);
    titleInput->setPlaceholderText("Enter book title");

    yearInput = new QSpinBox(this);
    yearInput->setMinimum(0);
    yearInput->setMaximum(QDate::currentDate().year());
    yearInput->clear();

    QPushButton* submitButton = new QPushButton("Submit", this);
    connect(submitButton, &QPushButton::clicked, this, &AddBookDialog::onSubmitClicked);

    // Layout
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(titleInput);
    layout->addWidget(yearInput);
    layout->addWidget(submitButton);

    setLayout(layout);
}

AddBookDialog::~AddBookDialog()
{
    delete ui;
}

void AddBookDialog::onSubmitClicked() {
    QString title = titleInput->text();
    int year = yearInput->value();
    emit bookSubmitted(title, year);
    this->accept();
}
