#ifndef USER_H
#define USER_H

#include "CommonUtil.h"

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

void addUser(sqlite3*& DB, QString forename, QString surname, QDate q_birth, QString email, QString phone = "");

bool getUserByID(sqlite3*& DB, int user_id, User& user);
std::vector<User> getUsers(sqlite3*& DB);

bool setUserForename(sqlite3*& DB, int user_id, QString newForename);
bool setUserSurname(sqlite3*& DB, int user_id, QString newSurname);
bool setUserBirth(sqlite3*& DB, int user_id, QDate q_newBirth);
bool setUserEmail(sqlite3*& DB, int user_id, QString newEmail);
bool setUserPhone(sqlite3*& DB, int user_id, QString newPhone);

void deleteUserByID(sqlite3*& DB, int user_id);

void sortUsers(std::vector<User>& users, Value value, bool bLess = true);

#endif // USER_H
