#include "addbookdialog.h"
#include "author.h"
#include "ui_addbookdialog.h"

AddBookDialog::AddBookDialog(sqlite3*& DB, QWidget *parent)
    : QDialog(parent)
    , DB(DB)
    , ui(new Ui::AddBookDialog)
{
    ui->setupUi(this);

    setWindowTitle("Add New Book");
    setModal(true);
    setFixedSize(300, 150);

    // Create input fields
    titleInput = new QLineEdit(this);
    titleInput->setPlaceholderText("Enter book title");

    yearInput = new QSpinBox(this);
    yearInput->setMinimum(0);
    yearInput->setMaximum(QDate::currentDate().year());
    yearInput->clear();

    authorDropdown = new QComboBox(this);
    populateAuthorDropdown(DB);
    connect(authorDropdown, QOverload<int>::of(&QComboBox::activated), this, &AddBookDialog::onAuthorChanged);

    QPushButton* submitButton = new QPushButton("Submit", this);
    connect(submitButton, &QPushButton::clicked, this, &AddBookDialog::onSubmitClicked);

    // Layout
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(titleInput);
    layout->addWidget(yearInput);
    layout->addWidget(authorDropdown);
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

void AddBookDialog::populateAuthorDropdown(sqlite3*& DB) {
    authorDropdown->clear();
    std::vector<Author> authors = getAuthors(DB);

    for (const Author& author : authors) {
        authorDropdown->addItem(author.forename + " " + author.surname, QVariant(author.id));
    }

    authorDropdown->addItem("Add new author...");
}

void AddBookDialog::onAuthorChanged(int index) {
    if (authorDropdown->itemText(index) == "Add new author...") {
        /*AddAuthorDialog dialog(this);
        if (dialog.exec() == QDialog::Accepted) {
            populateAuthorDropdown(DB); // refresh list
            authorDropdown->setCurrentIndex(authorDropdown->count() - 2); // select new author (assumes added last before "Add new author")
        } else {
            // Revert selection if cancelled
            authorDropdown->setCurrentIndex(0);
        }*/
    }
}
