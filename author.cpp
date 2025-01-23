#include "author.h"

bool operator==(const Author& author, const Author& other) {
    return author.id == other.id &&
           author.forename == other.forename &&
           author.surname == other.surname &&
           author.bio == other.bio &&
           author.birth == other.birth &&
           author.death == other.death;
}

void addAuthor(sqlite3*& DB, QString forename, QString surname, QString bio, QDate q_birth, QDate q_death) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO AUTHORS (FORENAME, SURNAME, BIO, BIRTH, DEATH) VALUES (?, ?, ?, ?, ?);";
    int exit = 0;

    QString birth = QDateToString(q_birth);
    QString death = QDateToString(q_death);

    exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        qCritical() << "Error preparing SQL statement1: " << sqlite3_errmsg(DB) << "\n";
        return;
    }

    sqlite3_bind_text(stmt, 1, forename.toUtf8().constData(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, surname.toUtf8().constData(), -1, SQLITE_STATIC);

    if (bio.isEmpty()) sqlite3_bind_null(stmt, 3);
    else sqlite3_bind_text(stmt, 3, bio.toUtf8().constData(), -1, SQLITE_STATIC);

    sqlite3_bind_text(stmt, 4, birth.toUtf8().constData(), -1, SQLITE_STATIC);

    if (death.isEmpty()) sqlite3_bind_null(stmt, 5);
    else sqlite3_bind_text(stmt, 5, death.toUtf8().constData(), -1, SQLITE_STATIC);

    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        qCritical() << "Error executing SQL statement2: " << sqlite3_errmsg(DB) << "\n";
    }
    else {
        qDebug() << "Author inserted successfully!";
    }

    sqlite3_finalize(stmt);
}

bool getAuthorByID(sqlite3*& DB, int author_id, Author& author) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT FORENAME, SURNAME, BIO, BIRTH, DEATH FROM AUTHORS WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

    sqlite3_bind_int(stmt, 1, author_id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* forename = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        const char* surname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

        const char* value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        QString bio = (value == nullptr) ? "" : QString(value);

        const char* birth = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

        value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        QString death = (value == nullptr) ? "" : QString(value);

        author = Author(author_id, forename, surname, bio, stringToQDate(birth), stringToQDate(death));
        sqlite3_finalize(stmt);
        return true;
    }
    sqlite3_finalize(stmt);
    return false;
}

std::vector<Author> getAuthors(sqlite3*& DB) {
    std::vector<Author> authors;
    sqlite3_stmt* stmt;
    const char* sql = "SELECT ID, FORENAME, SURNAME, BIO, BIRTH, DEATH FROM AUTHORS;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char* forename = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* surname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        const char* value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        QString bio = (value == nullptr) ? "" : QString(value);
        const char* b_date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

        value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        QString d_date = (value == nullptr) ? "" : QString(value);

        Author author(id, forename, surname, bio, stringToQDate(b_date), stringToQDate(d_date));
        authors.push_back(author);
    }
    sqlite3_finalize(stmt);
    return authors;
}

bool setAuthorForename(sqlite3*& DB, int author_id, QString newForename) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE AUTHORS SET FORENAME = ? WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, newForename.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, author_id);

    exit = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    return exit == SQLITE_DONE;
}

bool setAuthorSurname(sqlite3*& DB, int author_id, QString newSurname) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE AUTHORS SET SURNAME = ? WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, newSurname.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, author_id);

    exit = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    return exit == SQLITE_DONE;
}

bool setAuthorBio(sqlite3*& DB, int author_id, QString newBio) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE AUTHORS SET BIO = ? WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        return false;
    }

    if (newBio.isEmpty()) sqlite3_bind_null(stmt, 1);
    else sqlite3_bind_text(stmt, 1, newBio.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, author_id);

    exit = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    return exit == SQLITE_DONE;
}

bool setAuthorBirthDate(sqlite3*& DB, int author_id, QDate q_newBirthDate) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE AUTHORS SET BIRTH_DATE = ? WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        return false;
    }

    QString newBirthDate = QDateToString(q_newBirthDate);
    if (newBirthDate.isEmpty()) sqlite3_bind_null(stmt, 1);
    else sqlite3_bind_text(stmt, 1, newBirthDate.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, author_id);

    exit = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    return exit == SQLITE_DONE;
}

bool setAuthorDeathDate(sqlite3*& DB, int author_id, QDate q_newDeathDate) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE AUTHORS SET DEATH_DATE = ? WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        return false;
    }

    QString newDeathDate = QDateToString(q_newDeathDate);
    if (newDeathDate.isEmpty()) sqlite3_bind_null(stmt, 1);
    else sqlite3_bind_text(stmt, 1, newDeathDate.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, author_id);

    exit = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    return exit == SQLITE_DONE;
}

void deleteAuthorByID(sqlite3*& DB, int author_id) {
    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM AUTHORS WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        qCritical() << "Error preparing SQL statement: " << sqlite3_errmsg(DB) << "\n";
        return;
    }

    sqlite3_bind_int(stmt, 1, author_id);

    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        qCritical() << "Error executing SQL statement: " << sqlite3_errmsg(DB) << "\n";
    }

    sqlite3_finalize(stmt);
}

void sortAuthors(std::vector<Author>& authors, Value value, bool bLess) {
    switch (value) {
    case ID: {
        std::sort(authors.begin(), authors.end(), [bLess](const Author& a, const Author& b) {
            return bLess ? a.id < b.id : a.id > b.id;
        });
        break;
    }
    case FORENAME: {
        std::sort(authors.begin(), authors.end(), [bLess](const Author& a, const Author& b) {
            return bLess ? a.forename < b.forename : a.forename > b.forename;
        });
        break;
    }
    case SURNAME: {
        std::sort(authors.begin(), authors.end(), [bLess](const Author& a, const Author& b) {
            return bLess ? a.surname < b.surname : a.surname > b.surname;
        });
        break;
    }
    case BIRTH: {
        std::sort(authors.begin(), authors.end(), [bLess](const Author& a, const Author& b) {
            return bLess ? a.birth < b.birth : a.birth > b.birth;
        });
        break;
    }
    case DEATH: {
        std::sort(authors.begin(), authors.end(), [bLess](const Author& a, const Author& b) {
            return bLess ? a.death < b.death : a.death > b.death;
        });
        break;
    }
    default: {
        qDebug("WRONG VALUE FIELD");
        break;
    }
    }
}
