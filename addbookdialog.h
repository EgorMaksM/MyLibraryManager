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
#include <QTimer>
#include <QLabel>
#include <QFormLayout>
#include <QSignalBlocker>
#include <CommonUtil.h>

#include "addauthordialog.h"

namespace Ui {
class AddBookDialog;
}

class AddBookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddBookDialog(sqlite3*& DB, QWidget *parent = nullptr);
    ~AddBookDialog();

    void populateAuthorDropdown(sqlite3*& DB, QComboBox* authorDropdown);
    void onAuthorChanged(int index, QComboBox* authorDropdown);

    QList<int> getSelectedAuthorIds() const;

    QLineEdit* titleInput;
    QSpinBox* yearInput;
    QComboBox* authorDropdown;

    sqlite3*& DB;

signals:
    void bookSubmitted(const QString& title, int year, QList<int> authorIds);

private:
    QVBoxLayout* authorsLayout;
    QList<QComboBox*> authorDropdowns;

    void addAuthorDropdown();

private slots:
    void onSubmitClicked();
};

#endif // ADDBOOKDIALOG_H
