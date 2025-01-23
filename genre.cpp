#include "genre.h"

bool operator==(const Genre& genre, const Genre& other) {
    return genre.id == other.id &&
           genre.name == other.name;
}

void addGenre(sqlite3*& DB, QString name) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO GENRES (NAME) VALUES (?);";
    int exit = 0;

    exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        qCritical() << "Error preparing SQL statement: " << sqlite3_errmsg(DB) << "\n";
        return;
    }

    sqlite3_bind_text(stmt, 1, name.toUtf8().constData(), -1, SQLITE_STATIC);

    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        qCritical() << "Error executing SQL statement: " << sqlite3_errmsg(DB) << "\n";
    }
    else {
        qDebug() << "Genre inserted successfully!";
    }

    sqlite3_finalize(stmt);
}

bool getGenreByID(sqlite3*& DB, int genre_id, Genre& genre) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT NAME FROM GENRES WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

    sqlite3_bind_int(stmt, 1, genre_id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        genre = Genre(genre_id, name);
        sqlite3_finalize(stmt);
        return true;
    }
    sqlite3_finalize(stmt);
    return false;
}

std::vector<Genre> getGenres(sqlite3*& DB) {
    std::vector<Genre> genres;
    sqlite3_stmt* stmt;
    const char* sql = "SELECT ID, NAME FROM GENRES;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

        Genre genre(id, name);
        genres.push_back(genre);
    }
    sqlite3_finalize(stmt);
    return genres;
}

bool setGenreName(sqlite3*& DB, int genre_id, QString newName) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE GENRES SET NAME = ? WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, newName.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, genre_id);

    exit = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    return exit == SQLITE_DONE;
}

void deleteGenreByID(sqlite3*& DB, int genre_id) {
    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM GENRES WHERE ID = ?;";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        qCritical() << "Error preparing SQL statement: " << sqlite3_errmsg(DB) << "\n";
        return;
    }

    sqlite3_bind_int(stmt, 1, genre_id);

    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        qCritical() << "Error executing SQL statement: " << sqlite3_errmsg(DB) << "\n";
    }

    sqlite3_finalize(stmt);
}

void sortGenres(std::vector<Genre>& genres, Value value, bool bLess) {
    switch (value) {
    case ID: {
        std::sort(genres.begin(), genres.end(), [bLess](const Genre& a, const Genre& b) {
            return bLess ? a.id < b.id : a.id > b.id;
        });
        break;
    }
    case NAME: {
        std::sort(genres.begin(), genres.end(), [bLess](const Genre& a, const Genre& b) {
            return bLess ? a.name < b.name : a.name > b.name;
        });
        break;
    }
    default: {
        qDebug("WRONG VALUE FIELD");
        break;
    }
    }
}
