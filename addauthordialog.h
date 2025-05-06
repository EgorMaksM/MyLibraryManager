#ifndef ADDAUTHORDIALOG_H
#define ADDAUTHORDIALOG_H

#include <QLineEdit>
#include <QTextEdit>
#include <QDateEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialog>

namespace Ui {
class AddAuthorDialog;
}

class AddAuthorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddAuthorDialog(QWidget *parent = nullptr);
    ~AddAuthorDialog();

    QLineEdit* forenameInput;
    QLineEdit* surnameInput;
    QTextEdit* bioInput;
    QDateEdit* birthDateInput;
    QDateEdit* deathDateInput;

signals:
    void authorSubmitted(const QString& forename, const QString& surname, const QString& bio, const QDate& birth, const QDate& death = QDate());

private:
    Ui::AddAuthorDialog *ui;

private slots:
    void onSubmitClicked();
};

#endif // ADDAUTHORDIALOG_H
