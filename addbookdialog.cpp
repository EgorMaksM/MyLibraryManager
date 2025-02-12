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

    yearInput = new QLineEdit(this);
    yearInput->setPlaceholderText("Enter publication year");

    // Restrict year input to numbers
    yearInput->setValidator(new QIntValidator(0, 9999, this));

    // Create Submit button
    QPushButton* submitButton = new QPushButton("Submit", this);
    //connect(submitButton, &QPushButton::clicked, this, &AddBookDialog::onSubmitClicked);

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
