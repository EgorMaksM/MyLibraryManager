#ifndef ADDBOOKDIALOG_H
#define ADDBOOKDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QIntValidator>
#include <QPushButton>
#include <QVboxLayout>
#include <QDate>
#include <QComboBox>
#include <CommonUtil.h>

namespace Ui {
class AddBookDialog;
}

class AddBookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddBookDialog(sqlite3*& DB, QWidget *parent = nullptr);
    ~AddBookDialog();

    void populateAuthorDropdown(sqlite3*& DB);
    void onAuthorChanged(int index);

    QLineEdit* titleInput;
    QSpinBox* yearInput;
    QComboBox* authorDropdown;

    sqlite3*& DB;

signals:
    void bookSubmitted(const QString& title, int year);

private:
    Ui::AddBookDialog *ui;

private slots:
    void onSubmitClicked();
};

#endif // ADDBOOKDIALOG_H
