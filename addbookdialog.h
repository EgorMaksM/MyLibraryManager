#ifndef ADDBOOKDIALOG_H
#define ADDBOOKDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QIntValidator>
#include <QPushButton>
#include <QVboxLayout>

namespace Ui {
class AddBookDialog;
}

class AddBookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddBookDialog(QWidget *parent = nullptr);
    ~AddBookDialog();

private:
    Ui::AddBookDialog *ui;

    QLineEdit* titleInput;
    QLineEdit* yearInput;
};

#endif // ADDBOOKDIALOG_H
