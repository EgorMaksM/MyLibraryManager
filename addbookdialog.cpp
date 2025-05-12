#include "addbookdialog.h"
#include "author.h"

AddBookDialog::AddBookDialog(sqlite3*& DB, QWidget *parent)
    : QDialog(parent)
    , DB(DB)
{

    setWindowTitle("Add New Book");
    setModal(true);
    this->adjustSize();

    // Create input fields
    QFormLayout* formLayout = new QFormLayout;

    titleInput = new QLineEdit();

    yearInput = new QSpinBox();
    yearInput->setMinimum(0);
    yearInput->setMaximum(QDate::currentDate().year());
    yearInput->clear();

    authorsLayout = new QVBoxLayout();
    QHBoxLayout* firstAuthorLayout = new QHBoxLayout();

    authorDropdown = new QComboBox();
    authorDropdowns.append(authorDropdown);
    populateAuthorDropdown(DB, authorDropdown);
    connect(authorDropdown, QOverload<int>::of(&QComboBox::activated),
            this, [=](int index) {
                onAuthorChanged(index, authorDropdown);
            });

    firstAuthorLayout->addWidget(authorDropdown);

    QPushButton* newAuthorButton = new QPushButton("+");
    connect(newAuthorButton, &QPushButton::clicked, this, &AddBookDialog::addAuthorDropdown);
    newAuthorButton->setFixedSize(25, 25);
    firstAuthorLayout->addWidget(newAuthorButton);

    authorsLayout->addLayout(firstAuthorLayout);

    QPushButton* submitButton = new QPushButton("Add The Book", this);
    connect(submitButton, &QPushButton::clicked, this, &AddBookDialog::onSubmitClicked);

    // Layout
    QVBoxLayout* layout = new QVBoxLayout(this);

    formLayout->addRow("Book's Title:", titleInput);
    formLayout->addRow("Book's Year of Publication:", yearInput);
    formLayout->addRow("Book's Author(s):", authorsLayout);

    layout->addLayout(formLayout);
    layout->addWidget(submitButton);

    setLayout(layout);
}

AddBookDialog::~AddBookDialog()
{

}

void AddBookDialog::onSubmitClicked() {
    QString title = titleInput->text();
    int year = yearInput->value();
    QList<int> authorIds = getSelectedAuthorIds();
    emit bookSubmitted(title, year, authorIds);
    this->accept();
}

void AddBookDialog::populateAuthorDropdown(sqlite3*& DB, QComboBox* authorDropdown) {
    authorDropdown->clear();
    std::vector<Author> authors = getAuthors(DB);

    for (const Author& author : authors) {
        authorDropdown->addItem(author.forename + " " + author.surname, QVariant(author.id));
    }

    authorDropdown->addItem("Add new author...");
}

void AddBookDialog::onAuthorChanged(int index, QComboBox* authorDropdown) {
    if (authorDropdown->itemText(index) == "Add new author...") {
        AddAuthorDialog* dialog = new AddAuthorDialog(this);

        connect(dialog, &AddAuthorDialog::authorSubmitted, this, [=](QString forename, QString surname, QString bio, QDate birth, QDate death) {
            int id = addAuthor(DB, forename, surname, bio, birth, death);

            for (QComboBox* dropdown : authorDropdowns) {
                populateAuthorDropdown(DB, dropdown);
            }

            for (int i = 0; i < authorDropdown->count(); ++i) {
                QVariant data = authorDropdown->itemData(i);
                if (data.isValid() && data.toInt() == id) {
                    authorDropdown->setCurrentIndex(i);
                    break;
                }
            }
        });

        if (dialog->exec() != QDialog::Accepted) {
            authorDropdown->setCurrentIndex(0);
        }

        dialog->deleteLater();
    }
}

void AddBookDialog::addAuthorDropdown() {
    QHBoxLayout* localLayout = new QHBoxLayout();

    QComboBox* localAuthorDropdown = new QComboBox();
    populateAuthorDropdown(DB, localAuthorDropdown);
    connect(authorDropdown, QOverload<int>::of(&QComboBox::activated),
            this, [=](int index) {
                onAuthorChanged(index, authorDropdown);
            });

    QPushButton* deleteAuthorButton = new QPushButton("-");
    connect(deleteAuthorButton, &QPushButton::clicked, this, [=]() {
        while (localLayout->count() > 0) {
            QLayoutItem* item = localLayout->takeAt(0);
            if (QWidget* widget = item->widget()) {
                widget->deleteLater();
            }
            delete item;
        }

        authorsLayout->removeItem(localLayout);

        delete localLayout;

        authorsLayout->update();
        this->adjustSize();
    });
    deleteAuthorButton->setFixedSize(25, 25);

    localLayout->addWidget(localAuthorDropdown);
    localLayout->addWidget(deleteAuthorButton);

    authorsLayout->addLayout(localLayout);

    authorDropdowns.append(localAuthorDropdown);
}

QList<int> AddBookDialog::getSelectedAuthorIds() const {
    QList<int> selectedIds;

    for (QComboBox* dropdown : authorDropdowns) {
        int index = dropdown->currentIndex();
        if (index != -1 && dropdown->currentText() != "Add new author...") {
            int authorId = dropdown->itemData(index).toInt();
            selectedIds.append(authorId);
        }
    }

    return selectedIds;
}
