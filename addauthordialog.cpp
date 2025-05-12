#include "addauthordialog.h"
#include "ui_addauthordialog.h"

AddAuthorDialog::AddAuthorDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddAuthorDialog)
{
    ui->setupUi(this);

    setWindowTitle("Add New Author");
    setModal(true);
    this->adjustSize();

    // Create input fields
    QFormLayout* formLayout = new QFormLayout();
    forenameInput = new QLineEdit();

    surnameInput = new QLineEdit();

    bioInput = new QTextEdit();

    birthDateInput = new QDateEdit();
    birthDateInput->setCalendarPopup(true);
    birthDateInput->setDate(QDate::currentDate());

    QVBoxLayout* deathDataLayout = new QVBoxLayout();

    IsDead = new QCheckBox("Is Still Alive");
    IsDead->setChecked(true);

    deathDateInput = new QDateEdit();
    deathDateInput->setCalendarPopup(true);
    deathDateInput->setDate(QDate::currentDate());
    deathDateInput->setEnabled(false);

    QObject::connect(IsDead, &QCheckBox::toggled, deathDateInput, &QDateEdit::setDisabled);

    deathDataLayout->addWidget(IsDead);
    deathDataLayout->addWidget(deathDateInput);

    QPushButton* submitButton = new QPushButton("Add The Author", this);
    connect(submitButton, &QPushButton::clicked, this, &AddAuthorDialog::onSubmitClicked);

    // Layout
    QVBoxLayout* layout = new QVBoxLayout(this);
    formLayout->addRow("Author's Forename:", forenameInput);
    formLayout->addRow("Author's Surname:", surnameInput);
    formLayout->addRow("Author's Biography(Optional):", bioInput);
    formLayout->addRow("Author's Birth Date:", birthDateInput);
    formLayout->addRow("Author's Death Date:", deathDataLayout);

    layout->addLayout(formLayout);
    layout->addWidget(submitButton);

    setLayout(layout);
}

void AddAuthorDialog::onSubmitClicked() {
    QString forename = forenameInput->text();
    QString surname = surnameInput->text();
    QString bio = bioInput->toPlainText();
    QDate birth = birthDateInput->date();
    QDate death = QDate();
    if (!IsDead->isChecked()) death = deathDateInput->date();
    emit authorSubmitted(forename, surname, bio, birth, death);
    this->accept();
}

AddAuthorDialog::~AddAuthorDialog()
{
    delete ui;
}
