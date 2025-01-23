#ifndef LOAN_H
#define LOAN_H

#include "CommonUtil.h"

struct Loan {
    int id;
    int user_id;
    int book_id;
    QDate start;
    QDate end;

    Loan(int id = NULL, int user_id = NULL, int book_id = NULL, const QDate& start = QDate(), const QDate& end = QDate())
        : id(id), user_id(user_id), book_id(book_id), start(start), end(end)
    {
    }
};

bool operator==(const Loan& loan, const Loan& other);

void addLoan(sqlite3*& DB, int user_id, int book_id, QDate q_start, QDate q_end);

std::vector<Loan> getLoans(sqlite3*& DB);

#endif // LOAN_H
