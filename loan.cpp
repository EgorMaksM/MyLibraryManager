#include "loan.h"

bool operator==(const Loan& loan, const Loan& other) {
    return loan.id == other.id &&
           loan.user_id == other.user_id &&
           loan.book_id == other.book_id &&
           loan.start == other.start &&
           loan.end == other.end;
}

void addLoan(sqlite3*& DB, int user_id, int book_id, QDate q_start, QDate q_end) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO LOANS (USER_ID, BOOK_ID, START, END) VALUES (?, ?, ?, ?);";
    int exit = 0;

    QString start = QDateToString(q_start);
    QString end = QDateToString(q_end);

    exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        qCritical() << "Error preparing SQL statement: " << sqlite3_errmsg(DB) << "\n";
        return;
    }

    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, book_id);
    sqlite3_bind_text(stmt, 3, start.toUtf8().constData(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, end.toUtf8().constData(), -1, SQLITE_STATIC);

    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        qCritical() << "Error executing SQL statement: " << sqlite3_errmsg(DB) << "\n";
    }
    else {
        qDebug("Loan inserted successfully!");
    }

    sqlite3_finalize(stmt);
}

std::vector<Loan> getLoans(sqlite3*& DB) {
    std::vector<Loan> loans;
    sqlite3_stmt* stmt;
    const char* sql = "SELECT ID, USER_ID, BOOK_ID, START, END FROM LOANS;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        int user_id = sqlite3_column_int(stmt, 1);
        int book_id = sqlite3_column_int(stmt, 2);
        const char* start = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        const char* end = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

        Loan loan = Loan(id, user_id, book_id, stringToQDate(start), stringToQDate(end));
        loans.push_back(loan);
    }
    sqlite3_finalize(stmt);
    return loans;
}
