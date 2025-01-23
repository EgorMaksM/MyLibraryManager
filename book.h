#ifndef BOOK_H
#define BOOK_H

#include "CommonUtil.h"

struct Book {
    int id;
    QString title;
    unsigned short int year;

    Book(int id = NULL, const QString& title = "", unsigned short int year = NULL)
        : id(id), title(title), year(year)
    {
    }
};

bool operator==(const Book& book, const Book& other);

void addBook(sqlite3*& DB, QString title, unsigned short int year);

bool getBookByID(sqlite3*& DB, int book_id, Book& book);
std::vector<Book> getBooks(sqlite3*& DB);

bool setBookName(sqlite3*& DB, int book_id, QString newName);
bool setBookYear(sqlite3*& DB, int book_id, unsigned short int newYear);

void deleteBookByID(sqlite3*& DB, int book_id);

void sortBooks(std::vector<Book>& books, Value value, bool bLess = true);

#endif // BOOK_H
