#ifndef ADDBOOKDIALOG_H
#define ADDBOOKDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QIntValidator>
#include <QPushButton>
#include <QVboxLayout>
#include <QDate>

namespace Ui {
class AddBookDialog;
}

class AddBookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddBookDialog(QWidget *parent = nullptr);
    ~AddBookDialog();

    QLineEdit* titleInput;
    QSpinBox* yearInput;

signals:
    void bookSubmitted(const QString& title, int year);

private:
    Ui::AddBookDialog *ui;

private slots:
    void onSubmitClicked();
};

#endif // ADDBOOKDIALOG_H
