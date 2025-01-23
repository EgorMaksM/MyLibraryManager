#include "book.h"

bool operator==(const Book& book, const Book& other) {
    return book.id == other.id &&
           book.title == other.title &&
           book.year == other.year;
}

void addBook(sqlite3*& DB, QString title, unsigned short int year) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO BOOKS (TITLE, YEAR) VALUES (?, ?);";
    int exit = 0;

    exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        qCritical() << "Error preparing SQL statement: " << sqlite3_errmsg(DB) << "\n";
        return;
    }

    sqlite3_bind_text(stmt, 1, title.toUtf8().constData(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, year);

    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        qCritical() << "Error executing SQL statement: " << sqlite3_errmsg(DB) << "\n";
    }
    else {
        qDebug() << "Book inserted successfully!";
    }
    sqlite3_finalize(stmt);
}

bool getBookByID(sqlite3*& DB, int book_id, Book& book) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT TITLE, YEAR FROM BOOKS WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

    sqlite3_bind_int(stmt, 1, book_id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        int year = sqlite3_column_int(stmt, 1);
        book = Book(book_id, title, year);
        sqlite3_finalize(stmt);
        return true;
    }
    sqlite3_finalize(stmt);
    return false;
}

std::vector<Book> getBooks(sqlite3*& DB) {
    std::vector<Book> books;
    sqlite3_stmt* stmt;
    const char* sql = "SELECT ID, TITLE, YEAR FROM BOOKS;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char* title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        int year = sqlite3_column_int(stmt, 2);

        Book book(id, title, year);
        books.push_back(book);
    }
    sqlite3_finalize(stmt);
    return books;
}

bool setBookName(sqlite3*& DB, int book_id, QString newName) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE BOOKS SET TITLE = ? WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, newName.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, book_id);

    exit = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    return exit == SQLITE_DONE;
}

bool setBookYear(sqlite3*& DB, int book_id, unsigned short int newYear) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE BOOKS SET YEAR = ? WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, newYear);
    sqlite3_bind_int(stmt, 2, book_id);

    exit = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    return exit == SQLITE_DONE;
}

void deleteBookByID(sqlite3*& DB, int book_id) {
    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM BOOKS WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        qCritical() << "Error preparing SQL statement: " << sqlite3_errmsg(DB) << "\n";
        return;
    }

    sqlite3_bind_int(stmt, 1, book_id);

    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        qCritical() << "Error executing SQL statement: " << sqlite3_errmsg(DB) << "\n";
    }

    sqlite3_finalize(stmt);
}

void sortBooks(std::vector<Book>& books, Value value, bool bLess) {
    switch (value) {
    case ID: {
        std::sort(books.begin(), books.end(), [bLess](const Book& a, const Book& b) {
            return bLess ? a.id < b.id : a.id > b.id;
        });
        break;
    }
    case TITLE: {
        std::sort(books.begin(), books.end(), [bLess](const Book& a, const Book& b) {
            return bLess ? a.title < b.title : a.title > b.title;
        });
        break;
    }
    case YEAR: {
        std::sort(books.begin(), books.end(), [bLess](const Book& a, const Book& b) {
            return bLess ? a.year < b.year : a.year > b.year;
        });
        break;
    }
    default: {
        qDebug("WRONG VALUE FIELD");
        break;
    }
    }
}
