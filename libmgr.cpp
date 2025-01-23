#include "libmgr.h"

void initDB(sqlite3*& DB, char*& messageError) {

    int exit = 0;
    exit = sqlite3_open("library.db", &DB);

    QString sql_books = R"(
        CREATE TABLE IF NOT EXISTS BOOKS(
        ID INTEGER PRIMARY KEY AUTOINCREMENT,
        TITLE      TINYTEXT NOT NULL,
        YEAR       INT UNSIGNED NOT NULL);
    )";

    QString sql_authors = R"(
        CREATE TABLE IF NOT EXISTS AUTHORS(
        ID INTEGER PRIMARY KEY AUTOINCREMENT,
        FORENAME   TINYTEXT NOT NULL,
        SURNAME    TINYTEXT NOT NULL,
        BIO        TEXT,
        BIRTH      DATE NOT NULL,
        DEATH      DATE);
    )";

    QString sql_book_authors = R"(
        CREATE TABLE IF NOT EXISTS BOOK_AUTHORS(
        ID INTEGER PRIMARY KEY AUTOINCREMENT,
        BOOK_ID    INTEGER NOT NULL,
        AUTHOR_ID  INTEGER NOT NULL,
        FOREIGN KEY (BOOK_ID) REFERENCES BOOKS(ID) ON DELETE CASCADE,
        FOREIGN KEY (AUTHOR_ID) REFERENCES AUTHORS(ID) ON DELETE CASCADE);
    )";

    QString sql_genres = R"(
        CREATE TABLE IF NOT EXISTS GENRES(
        ID INTEGER PRIMARY KEY AUTOINCREMENT,
        NAME       TINYTEXT NOT NULL);
    )";

    QString sql_book_genres = R"(
        CREATE TABLE IF NOT EXISTS BOOK_GENRES(
        ID INTEGER PRIMARY KEY AUTOINCREMENT,
        BOOK_ID    INTEGER NOT NULL,
        GENRE_ID   INTEGER NOT NULL,
        FOREIGN KEY (BOOK_ID) REFERENCES BOOKS(ID) ON DELETE CASCADE,
        FOREIGN KEY (GENRE_ID) REFERENCES GENRES(ID) ON DELETE CASCADE);
    )";

    QString sql_users = R"(
        CREATE TABLE IF NOT EXISTS USERS(
        ID INTEGER PRIMARY KEY AUTOINCREMENT,
        FORENAME       TINYTEXT NOT NULL,
        SURNAME        TINYTEXT NOT NULL,
        BIRTH          DATE NOT NULL,
        EMAIL          TINYTEXT NOT NULL,
        PHONE          TINYTEXT);
    )";

    QString sql_loans = R"(
        CREATE TABLE IF NOT EXISTS LOANS(
        ID INTEGER PRIMARY KEY AUTOINCREMENT,
        USER_ID       INTEGER NOT NULL,
        BOOK_ID       INTEGER NOT NULL,
        START         DATE NOT NULL,
        END           DATE NOT NULL,
        FOREIGN KEY (USER_ID) REFERENCES USERS(ID) ON DELETE CASCADE,
        FOREIGN KEY (BOOK_ID) REFERENCES BOOKS(ID) ON DELETE CASCADE);
    )";


    exit = sqlite3_exec(DB, sql_books.toUtf8().constData(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        qCritical() << "Error in create BOOKS." << "\n";
        sqlite3_free(messageError);
    }

    exit = sqlite3_exec(DB, sql_authors.toUtf8().constData(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        qCritical() << "Error in create AUTHORS." << "\n";
        sqlite3_free(messageError);
    }

    exit = sqlite3_exec(DB, sql_book_authors.toUtf8().constData(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        qCritical() << "Error in create BOOK_AUTHORS." << "\n";
        sqlite3_free(messageError);
    }

    exit = sqlite3_exec(DB, sql_genres.toUtf8().constData(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        qCritical() << "Error in create GENRES." << "\n";
        sqlite3_free(messageError);
    }

    exit = sqlite3_exec(DB, sql_book_genres.toUtf8().constData(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        qCritical() << "Error in create BOOK_GENRES" << "\n";
        sqlite3_free(messageError);
    }

    exit = sqlite3_exec(DB, sql_users.toUtf8().constData(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        qCritical() << "Error in create USERS" << "\n";
        sqlite3_free(messageError);
    }

    exit = sqlite3_exec(DB, sql_loans.toUtf8().constData(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        qCritical() << "Error in create LOANS" << "\n";
        sqlite3_free(messageError);
    }
}

void linkAuthorToBook(sqlite3*& DB, int author_id, int book_id) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO BOOK_AUTHORS (BOOK_ID, AUTHOR_ID) VALUES (?, ?);";
    int exit = 0;

    exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        qCritical() << "Error preparing SQL statement: " << sqlite3_errmsg(DB) << "\n";
        return;
    }

    sqlite3_bind_int(stmt, 1, book_id);
    sqlite3_bind_int(stmt, 2, author_id);

    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        qCritical() << "Error executing SQL statement: " << sqlite3_errmsg(DB) << "\n";
    }
    else {
        qDebug("Book<->Author relationship inserted successfully!");
    }

    sqlite3_finalize(stmt);
}

void linkGenreToBook(sqlite3*& DB, int genre_id, int book_id) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO BOOK_GENRES (BOOK_ID, GENRE_ID) VALUES (?, ?);";
    int exit = 0;

    exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        qCritical() << "Error preparing SQL statement: " << sqlite3_errmsg(DB) << "\n";
        return;
    }

    sqlite3_bind_int(stmt, 1, book_id);
    sqlite3_bind_int(stmt, 2, genre_id);

    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        qCritical() << "Error executing SQL statement: " << sqlite3_errmsg(DB) << "\n";
    }
    else {
        qDebug("Book<->Genre relationship inserted successfully!");
    }

    sqlite3_finalize(stmt);
}

bool unlinkAuthorFromBook(sqlite3*& DB, int author_id, int book_id) {
    sqlite3_stmt* stmt;
    const char* sql = R"(
        DELETE FROM BOOK_AUTHORS
        WHERE BOOK_ID = ? AND AUTHOR_ID = ?;
    )";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr);
    if (exit != SQLITE_OK) {
        qCritical() << "Error preparing statement: " << sqlite3_errmsg(DB) << "\n";
        return false;
    }

    sqlite3_bind_int(stmt, 1, book_id);
    sqlite3_bind_int(stmt, 2, author_id);

    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        qCritical() << "Error executing statement: " << sqlite3_errmsg(DB) << "\n";
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

bool unlinkGenreFromBook(sqlite3*& DB, int genre_id, int book_id) {
    sqlite3_stmt* stmt;
    const char* sql = R"(
        DELETE FROM BOOK_GENRES
        WHERE BOOK_ID = ? AND GENRE_ID = ?;
    )";

    int exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, nullptr);
    if (exit != SQLITE_OK) {
        qCritical() << "Error preparing statement: " << sqlite3_errmsg(DB) << "\n";
        return false;
    }

    sqlite3_bind_int(stmt, 1, book_id);
    sqlite3_bind_int(stmt, 2, genre_id);

    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        qCritical() << "Error executing statement: " << sqlite3_errmsg(DB) << "\n";
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

bool getAuthorsByBookID(sqlite3*& DB, int book_id, std::vector<Author>& authors) {
    sqlite3_stmt* stmt;
    const char* sql = R"(
        SELECT BOOK_AUTHORS.AUTHOR_ID
        FROM BOOK_AUTHORS
        WHERE BOOK_AUTHORS.BOOK_ID = ?;
    )";
    int exit = 0;

    exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

    sqlite3_bind_int(stmt, 1, book_id);

    std::vector<int> result;
    authors.clear();
    while ((exit = sqlite3_step(stmt)) == SQLITE_ROW) {
        result.push_back(sqlite3_column_int(stmt, 0));
    }
    sqlite3_finalize(stmt);
    for (int i = 0; i < result.size(); i++) {
        Author author;
        if (!getAuthorByID(DB, result[i], author)) return false;
        authors.push_back(author);
    }
    return true;
}

bool getBooksByAuthorID(sqlite3*& DB, int author_id, std::vector<Book>& books) {
    sqlite3_stmt* stmt;
    const char* sql = R"(
        SELECT BOOK_AUTHORS.BOOK_ID
        FROM BOOK_AUTHORS
        WHERE BOOK_AUTHORS.AUTHOR_ID = ?;
    )";
    int exit = 0;

    exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

    sqlite3_bind_int(stmt, 1, author_id);

    std::vector<int> result;
    while ((exit = sqlite3_step(stmt)) == SQLITE_ROW) {
        result.push_back(sqlite3_column_int(stmt, 0));
    }
    sqlite3_finalize(stmt);
    for (int i = 0; i < result.size(); i++) {
        Book book;
        if (!getBookByID(DB, result[i], book)) return false;
        books.push_back(book);
    }
    return true;
}

bool getGenresByBookID(sqlite3*& DB, int book_id, std::vector<Genre>& genres) {
    sqlite3_stmt* stmt;
    const char* sql = R"(
        SELECT BOOK_GENRES.GENRE_ID
        FROM BOOK_GENRES
        WHERE BOOK_GENRES.BOOK_ID = ?;
    )";
    int exit = 0;

    exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

    sqlite3_bind_int(stmt, 1, book_id);

    std::vector<int> result;
    while ((exit = sqlite3_step(stmt)) == SQLITE_ROW) {
        result.push_back(sqlite3_column_int(stmt, 0));
    }
    sqlite3_finalize(stmt);
    for (int i = 0; i < result.size(); i++) {
        Genre genre;
        if (!getGenreByID(DB, result[i], genre)) return false;
        genres.push_back(genre);
    }
    return true;
}

bool getBooksByGenreID(sqlite3*& DB, int genre_id, std::vector<Book>& books) {
    sqlite3_stmt* stmt;
    const char* sql = R"(
        SELECT BOOK_GENRES.BOOK_ID
        FROM BOOK_GENRES
        WHERE BOOK_GENRES.GENRE_ID = ?;
    )";
    int exit = 0;

    exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

    sqlite3_bind_int(stmt, 1, genre_id);

    std::vector<int> result;
    while ((exit = sqlite3_step(stmt)) == SQLITE_ROW) {
        result.push_back(sqlite3_column_int(stmt, 0));
    }
    sqlite3_finalize(stmt);
    for (int i = 0; i < result.size(); i++) {
        Book book;
        if (!getBookByID(DB, result[i], book)) return false;
        books.push_back(book);
    }
    return true;
}

bool getGenresByAuthorID(sqlite3*& DB, int author_id, std::vector<Genre>& genres) {
    genres.clear();
    std::vector<Book> authorBooks;
    if (!getBooksByAuthorID(DB, author_id, authorBooks)) return false;

    for (int i = 0; i < authorBooks.size(); i++) {
        std::vector<Genre> searchVec;
        if (!getGenresByBookID(DB, authorBooks[i].id, searchVec)) return false;
        for (int j = 0; j < searchVec.size(); j++) {
            if (std::find(genres.begin(), genres.end(), searchVec[j]) == genres.end())
                genres.push_back(searchVec[j]);
        }
    }
    return true;
}

bool getBooksByUserID(sqlite3*& DB, int user_id, std::vector<Book>& books) {
    sqlite3_stmt* stmt;
    const char* sql = R"(
        SELECT LOANS.BOOK_ID
        FROM LOANS
        WHERE LOANS.USER_ID = ?;
    )";
    int exit = 0;

    exit = sqlite3_prepare_v2(DB, sql, -1, &stmt, 0);

    sqlite3_bind_int(stmt, 1, user_id);

    std::vector<int> result;
    while ((exit = sqlite3_step(stmt)) == SQLITE_ROW) {
        result.push_back(sqlite3_column_int(stmt, 0));
    }
    sqlite3_finalize(stmt);
    for (int i = 0; i < result.size(); i++) {
        Book book;
        if (!getBookByID(DB, result[i], book)) return false;
        books.push_back(book);
    }
    return true;
}
