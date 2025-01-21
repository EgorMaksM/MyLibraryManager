#ifndef LIBMGR_H
#define LIBMGR_H

#include <sqlite3.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <QDate>

QDate stringToQDate(const std::string& dateStr);

std::string QDateToString(const QDate& date);

struct Book {
    int id;
    std::string title;
    unsigned short int year;

    Book(int id = NULL, const std::string& title = "", unsigned short int year = NULL)
        : id(id), title(title), year(year)
    {
    }

    bool operator==(const Book& other) const;
};

struct Author {
    int id;
    std::string forename;
    std::string surname;
    std::string bio;
    QDate birth;
    QDate death;

    Author(int id = NULL, const std::string& forename = "", const std::string& surname = "", const std::string& bio = "", const QDate& birth = QDate(), const QDate& death = QDate())
        : id(id), forename(forename), surname(surname), bio(bio), birth(birth), death(death)
    {
    }
};

bool operator==(const Author& author, const Author& other);

struct Genre {
    int id;
    std::string name;

    Genre(int id = NULL, const std::string& name = "")
        : id(id), name(name)
    {
    }
};

bool operator==(const Genre& genre, const Genre& other);

struct User {
    int id;
    std::string forename;
    std::string surname;
    QDate birth;
    std::string email;
    std::string phone;

    User(int id = NULL, const std::string& forename = "", const std::string& surname = "", const QDate& birth = QDate(), const std::string& email = "", const std::string& phone = "")
        : id(id), forename(forename), surname(surname), birth(birth), email(email), phone(phone)
    {
    }
};

bool operator==(const User& user, const User& other);

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

enum Value {
    ID,
    TITLE,
    YEAR,
    FORENAME,
    SURNAME,
    BIRTH,
    DEATH,
    NAME
};

int callback(void* data, int argc, char** argv, char** azColName);
void printTable(sqlite3*& DB, std::string name);
void say(std::string x);
void initDB(sqlite3*& DB, char*& messageError);
void IOHandle(sqlite3*& DB);
void addBook(sqlite3*& DB, std::string title, unsigned short int year);
void addAuthor(sqlite3*& DB, std::string forename, std::string surname, std::string bio, QDate q_birth, QDate q_death);
void addGenre(sqlite3*& DB, std::string name);
void addUser(sqlite3*& DB, std::string forename, std::string surname, QDate q_birth, std::string email, std::string phone = "");
void addLoan(sqlite3*& DB, int user_id, int book_id, QDate q_start, QDate q_end);
void linkAuthorToBook(sqlite3*& DB, int author_id, int book_id);
void linkGenreToBook(sqlite3*& DB, int genre_id, int book_id);
bool unlinkAuthorFromBook(sqlite3*& DB, int author_id, int book_id);
bool unlinkGenreFromBook(sqlite3*& DB, int genre_id, int book_id);

bool getAuthorsByBookID(sqlite3*& DB, int book_id, std::vector<Author>& authors);
bool getBooksByAuthorID(sqlite3*& DB, int author_id, std::vector<Book>& books);
bool getGenresByBookID(sqlite3*& DB, int book_id, std::vector<Genre>& genres);
bool getBooksByGenreID(sqlite3*& DB, int genre_id, std::vector<Book>& books);
bool getGenresByAuthorID(sqlite3*& DB, int author_id, std::vector<Genre>& genres);
bool getBooksByUserID(sqlite3*& DB, int user_id, std::vector<Book>& books);

bool getBookByID(sqlite3*& DB, int book_id, Book& book);
bool getAuthorByID(sqlite3*& DB, int author_id, Author& author);
bool getGenreByID(sqlite3*& DB, int genre_id, Genre& genre);
bool getUserByID(sqlite3*& DB, int user_id, User& user);

bool setBookName(sqlite3*& DB, int book_id, std::string newName);
bool setBookYear(sqlite3*& DB, int book_id, unsigned short int newYear);
bool setAuthorForename(sqlite3*& DB, int author_id, std::string newForename);
bool setAuthorSurname(sqlite3*& DB, int author_id, std::string newSurname);
bool setAuthorBio(sqlite3*& DB, int author_id, std::string newBio);
bool setAuthorBirthDate(sqlite3*& DB, int author_id, QDate q_newBirthDate);
bool setAuthorDeathDate(sqlite3*& DB, int author_id, QDate q_newDeathDate);
bool setGenreName(sqlite3*& DB, int genre_id, std::string newName);
bool setUserForename(sqlite3*& DB, int user_id, std::string newForename);
bool setUserSurname(sqlite3*& DB, int user_id, std::string newSurname);
bool setUserBirth(sqlite3*& DB, int user_id, QDate q_newBirth);
bool setUserEmail(sqlite3*& DB, int user_id, std::string newEmail);
bool setUserPhone(sqlite3*& DB, int user_id, std::string newPhone);

void deleteBookByID(sqlite3*& DB, int book_id);
void deleteAuthorByID(sqlite3*& DB, int author_id);
void deleteGenreByID(sqlite3*& DB, int genre_id);
void deleteUserByID(sqlite3*& DB, int user_id);

std::vector<Book> sortBooks(sqlite3*& DB, Value value, bool bLess = true);
void sortBooks(std::vector<Book>& books, Value value, bool bLess = true);
std::vector<Author> sortAuthors(sqlite3*& DB, Value value, bool bLess = true);
void sortAuthors(std::vector<Author>& authors, Value value, bool bLess = true);
std::vector<Genre> sortGenres(sqlite3*& DB, Value value, bool bLess = true);
void sortGenres(std::vector<Genre>& genres, Value value, bool bLess = true);
std::vector<User> sortUsers(sqlite3*& DB, Value value, bool bLess = true);
void sortUsers(std::vector<User>& users, Value value, bool bLess = true);

#endif // LIBMGR_H
