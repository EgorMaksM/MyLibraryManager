#ifndef LIBMGR_H
#define LIBMGR_H

#include <sqlite3.h>
#include <vector>
#include <algorithm>

#include <QDate>

QDate stringToQDate(const QString& dateStr);

QString QDateToString(const QDate& date);

struct Book {
    int id;
    QString title;
    unsigned short int year;

    Book(int id = NULL, const QString& title = "", unsigned short int year = NULL)
        : id(id), title(title), year(year)
    {
    }

    bool operator==(const Book& other) const;
};

struct Author {
    int id;
    QString forename;
    QString surname;
    QString bio;
    QDate birth;
    QDate death;

    Author(int id = NULL, const QString& forename = "", const QString& surname = "", const QString& bio = "", const QDate& birth = QDate(), const QDate& death = QDate())
        : id(id), forename(forename), surname(surname), bio(bio), birth(birth), death(death)
    {
    }
};

bool operator==(const Author& author, const Author& other);

struct Genre {
    int id;
    QString name;

    Genre(int id = NULL, const QString& name = "")
        : id(id), name(name)
    {
    }
};

bool operator==(const Genre& genre, const Genre& other);

struct User {
    int id;
    QString forename;
    QString surname;
    QDate birth;
    QString email;
    QString phone;

    User(int id = NULL, const QString& forename = "", const QString& surname = "", const QDate& birth = QDate(), const QString& email = "", const QString& phone = "")
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

void initDB(sqlite3*& DB, char*& messageError);

void addBook(sqlite3*& DB, QString title, unsigned short int year);
void addAuthor(sqlite3*& DB, QString forename, QString surname, QString bio, QDate q_birth, QDate q_death);
void addGenre(sqlite3*& DB, QString name);
void addUser(sqlite3*& DB, QString forename, QString surname, QDate q_birth, QString email, QString phone = "");
void addLoan(sqlite3*& DB, int user_id, int book_id, QDate q_start, QDate q_end);

void linkAuthorToBook(sqlite3*& DB, int author_id, int book_id);
void linkGenreToBook(sqlite3*& DB, int genre_id, int book_id);
bool unlinkAuthorFromBook(sqlite3*& DB, int author_id, int book_id);
bool unlinkGenreFromBook(sqlite3*& DB, int genre_id, int book_id);

std::vector<Book> getBooks(sqlite3*& DB);
std::vector<Author> getAuthors(sqlite3*& DB);
std::vector<Genre> getGenres(sqlite3*& DB);
std::vector<User> getUsers(sqlite3*& DB);
std::vector<Loan> getLoans(sqlite3*& DB);

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

bool setBookName(sqlite3*& DB, int book_id, QString newName);
bool setBookYear(sqlite3*& DB, int book_id, unsigned short int newYear);
bool setAuthorForename(sqlite3*& DB, int author_id, QString newForename);
bool setAuthorSurname(sqlite3*& DB, int author_id, QString newSurname);
bool setAuthorBio(sqlite3*& DB, int author_id, QString newBio);
bool setAuthorBirthDate(sqlite3*& DB, int author_id, QDate q_newBirthDate);
bool setAuthorDeathDate(sqlite3*& DB, int author_id, QDate q_newDeathDate);
bool setGenreName(sqlite3*& DB, int genre_id, QString newName);
bool setUserForename(sqlite3*& DB, int user_id, QString newForename);
bool setUserSurname(sqlite3*& DB, int user_id, QString newSurname);
bool setUserBirth(sqlite3*& DB, int user_id, QDate q_newBirth);
bool setUserEmail(sqlite3*& DB, int user_id, QString newEmail);
bool setUserPhone(sqlite3*& DB, int user_id, QString newPhone);

void deleteBookByID(sqlite3*& DB, int book_id);
void deleteAuthorByID(sqlite3*& DB, int author_id);
void deleteGenreByID(sqlite3*& DB, int genre_id);
void deleteUserByID(sqlite3*& DB, int user_id);

void sortBooks(std::vector<Book>& books, Value value, bool bLess = true);
void sortAuthors(std::vector<Author>& authors, Value value, bool bLess = true);
void sortGenres(std::vector<Genre>& genres, Value value, bool bLess = true);
void sortUsers(std::vector<User>& users, Value value, bool bLess = true);

#endif // LIBMGR_H
