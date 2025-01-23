#include "user.h"

bool operator==(const User& user, const User& other) {
    return user.id == other.id &&
           user.forename == other.forename &&
           user.surname == other.surname &&
           user.birth == other.birth &&
           user.email == other.email &&
           user.phone == other.phone;
}

void addUser(sqlite3*& DB, QString forename, QString surname, QDate q_birth, QString email, QString phone) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO USERS (FORENAME, SURNAME, BIRTH, EMAIL, PHONE) VALUES (?, ?, ?, ?, ?);";
    int exit = 0;

    QString birth = QDateToString(q_birth);
    exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        qCritical() << "Error preparing SQL statement: " << sqlite3_errmsg(DB) << "\n";
        return;
    }

    sqlite3_bind_text(stmt, 1, forename.toUtf8().constData(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, surname.toUtf8().constData(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, birth.toUtf8().constData(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, email.toUtf8().constData(), -1, SQLITE_STATIC);
    if (phone.isEmpty()) sqlite3_bind_null(stmt, 5);
    else sqlite3_bind_text(stmt, 5, phone.toUtf8().constData(), -1, SQLITE_STATIC);
    //sqlite3_bind_text(stmt, 5, phone.toUtf8().constData(), -1, SQLITE_STATIC);

    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        qCritical() << "Error executing SQL statement: " << sqlite3_errmsg(DB) << "\n";
    }
    else {
        qDebug("User inserted successfully!");
    }

    sqlite3_finalize(stmt);
}

bool getUserByID(sqlite3*& DB, int user_id, User& user) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT FORENAME, SURNAME, BIRTH, EMAIL, PHONE FROM USERS WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

    sqlite3_bind_int(stmt, 1, user_id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* forename = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        const char* surname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* birth = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        const char* email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

        const char* value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        QString phone = (value == nullptr) ? "" : QString(value);
        user = User(user_id, forename, surname, stringToQDate(birth), email, phone);
        sqlite3_finalize(stmt);
        return true;
    }
    sqlite3_finalize(stmt);
    return false;
}

std::vector<User> getUsers(sqlite3*& DB) {
    std::vector<User> users;
    sqlite3_stmt* stmt;
    const char* sql = "SELECT ID, FORENAME, SURNAME, BIRTH, EMAIL, PHONE FROM USERS;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char* forename = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* surname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        const char* birth = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        const char* email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

        const char* value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        QString phone = (value == nullptr) ? "" : QString(value);

        User user = User(id, forename, surname, stringToQDate(birth), email, phone);
        users.push_back(user);
    }
    sqlite3_finalize(stmt);
    return users;
}

bool setUserForename(sqlite3*& DB, int user_id, QString newForename) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE USERS SET FORENAME = ? WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, newForename.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, user_id);

    exit = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    return exit == SQLITE_DONE;
}

bool setUserSurname(sqlite3*& DB, int user_id, QString newSurname) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE USERS SET SURNAME = ? WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, newSurname.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, user_id);

    exit = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    return exit == SQLITE_DONE;
}

bool setUserBirth(sqlite3*& DB, int user_id, QDate q_newBirth) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE USERS SET BIRTH = ? WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        return false;
    }

    QString newBirth = QDateToString(q_newBirth);
    sqlite3_bind_text(stmt, 1, newBirth.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, user_id);

    exit = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    return exit == SQLITE_DONE;
}

bool setUserEmail(sqlite3*& DB, int user_id, QString newEmail) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE USERS SET EMAIL = ? WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, newEmail.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, user_id);

    exit = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    return exit == SQLITE_DONE;
}

bool setUserPhone(sqlite3*& DB, int user_id, QString newPhone) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE USERS SET PHONE = ? WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        return false;
    }

    if (newPhone.isEmpty()) sqlite3_bind_null(stmt, 1);
    else sqlite3_bind_text(stmt, 1, newPhone.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, user_id);

    exit = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    return exit == SQLITE_DONE;
}

void deleteUserByID(sqlite3*& DB, int user_id) {
    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM USERS WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        qCritical() << "Error preparing SQL statement: " << sqlite3_errmsg(DB) << "\n";
        return;
    }

    sqlite3_bind_int(stmt, 1, user_id);

    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        qCritical() << "Error executing SQL statement: " << sqlite3_errmsg(DB) << "\n";
    }

    sqlite3_finalize(stmt);
}

void sortUsers(std::vector<User>& users, Value value, bool bLess) {
    switch (value) {
    case ID: {
        std::sort(users.begin(), users.end(), [bLess](const User& a, const User& b) {
            return bLess ? a.id < b.id : a.id > b.id;
        });
        break;
    }
    case FORENAME: {
        std::sort(users.begin(), users.end(), [bLess](const User& a, const User& b) {
            return bLess ? a.forename < b.forename : a.forename > b.forename;
        });
        break;
    }
    case SURNAME: {
        std::sort(users.begin(), users.end(), [bLess](const User& a, const User& b) {
            return bLess ? a.surname < b.surname : a.surname > b.surname;
        });
        break;
    }
    case BIRTH: {
        std::sort(users.begin(), users.end(), [bLess](const User& a, const User& b) {
            return bLess ? a.birth < b.birth : a.birth > b.birth;
        });
        break;
    }
    default: {
        qDebug("WRONG VALUE FIELD");
        break;
    }
    }
}
