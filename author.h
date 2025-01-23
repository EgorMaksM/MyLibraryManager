#ifndef AUTHOR_H
#define AUTHOR_H

#include "CommonUtil.h"

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

void addAuthor(sqlite3*& DB, QString forename, QString surname, QString bio, QDate q_birth, QDate q_death);

bool getAuthorByID(sqlite3*& DB, int author_id, Author& author);
std::vector<Author> getAuthors(sqlite3*& DB);

bool setAuthorForename(sqlite3*& DB, int author_id, QString newForename);
bool setAuthorSurname(sqlite3*& DB, int author_id, QString newSurname);
bool setAuthorBio(sqlite3*& DB, int author_id, QString newBio);
bool setAuthorBirthDate(sqlite3*& DB, int author_id, QDate q_newBirthDate);
bool setAuthorDeathDate(sqlite3*& DB, int author_id, QDate q_newDeathDate);

void deleteAuthorByID(sqlite3*& DB, int author_id);

void sortAuthors(std::vector<Author>& authors, Value value, bool bLess = true);

#endif // AUTHOR_H
